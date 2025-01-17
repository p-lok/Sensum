﻿#include "features.h"
#include "../globals.h"
#include "../render/render.h"
#include "../helpers/imdraw.h"
#include "../helpers/console.h"
#include "../esp.hpp"
#include "../hooks/hooks.h"
#include "..//runtime_saver.h"
#include "..//render/render.h"
#include "..//Backtrack_new.h"
#include "..//helpers/autowall.h"

#include <mutex>

extern float side;

QAngle viewanglesBackup;

namespace visuals
{
	std::mutex render_mutex;

	struct entity_data_t
	{
		std::string text;
		Vector origin;
		Color color;
	};

	struct grenade_info_t
	{
		std::string name;
		Color color;
	};

	RECT GetBBox(c_base_entity* ent)
	{
		RECT rect{};
		auto collideable = ent->GetCollideable();

		if (!collideable)
			return rect;

		auto min = collideable->OBBMins();
		auto max = collideable->OBBMaxs();

		const matrix3x4_t& trans = ent->m_rgflCoordinateFrame();

		Vector points[] =
		{
			Vector(min.x, min.y, min.z),
			Vector(min.x, max.y, min.z),
			Vector(max.x, max.y, min.z),
			Vector(max.x, min.y, min.z),
			Vector(max.x, max.y, max.z),
			Vector(min.x, max.y, max.z),
			Vector(min.x, min.y, max.z),
			Vector(max.x, min.y, max.z)
		};

		Vector pointsTransformed[8];

		for (int i = 0; i < 8; i++)
			math::VectorTransform(points[i], trans, pointsTransformed[i]);

		Vector screen_points[8] = {};

		for (int i = 0; i < 8; i++)
		{
			if (!math::world2screen(pointsTransformed[i], screen_points[i]))
				return rect;
		}

		auto left = screen_points[0].x;
		auto top = screen_points[0].y;
		auto right = screen_points[0].x;
		auto bottom = screen_points[0].y;

		for (int i = 1; i < 8; i++)
		{
			if (left > screen_points[i].x)
				left = screen_points[i].x;

			if (top < screen_points[i].y)
				top = screen_points[i].y;

			if (right < screen_points[i].x)
				right = screen_points[i].x;

			if (bottom > screen_points[i].y)
				bottom = screen_points[i].y;
		}

		return RECT{ (long)left, (long)top, (long)right, (long)bottom };
	}

	std::vector<entity_data_t> entities;
	std::vector<entity_data_t> saved_entities;

	bool is_enabled()
	{
		return interfaces::engine_client->IsConnected() && interfaces::local_player && !render::menu::is_visible();
	}

	void push_entity(c_base_entity* entity, const std::string& text, const Color& color = Color::White)
	{
		entities.emplace_back(entity_data_t{ text, entity->m_vecOrigin(), color });
	}

	void world_grenades(c_base_player* entity)
	{
		if (!interfaces::local_player || !interfaces::local_player->IsAlive())
			return;

		if (interfaces::local_player->IsFlashed())
			return;

		/*if (!interfaces::local_player->CanSeePlayer(entity, entity->GetRenderOrigin()))
			return; */

		if (utils::is_line_goes_through_smoke(interfaces::local_player->GetEyePos(), entity->GetRenderOrigin()))
			return;

		auto bbox = GetBBox(entity);

		grenade_info_t info;
		const auto model_name = fnv::hash_runtime(interfaces::mdl_info->GetModelName(entity->GetModel()));
		if (model_name == FNV("models/Weapons/w_eq_smokegrenade_thrown.mdl"))
			info = { "Smoke", Color::White };
		else if (model_name == FNV("models/Weapons/w_eq_flashbang_dropped.mdl"))
			info = { "Flash", Color::Yellow };
		else if (model_name == FNV("models/Weapons/w_eq_fraggrenade_dropped.mdl"))
			info = { "Grenade", Color::Red };
		else if (model_name == FNV("models/Weapons/w_eq_molotov_dropped.mdl") || model_name == FNV("models/Weapons/w_eq_incendiarygrenade_dropped.mdl"))
			info = { "Molly", Color::Orange };
		else if (model_name == FNV("models/Weapons/w_eq_decoy_dropped.mdl"))
			info = { "Decoy", Color::Green };

		if (!info.name.empty())
			push_entity(entity, info.name, info.color);
	}

	void DrawDamageIndicator()
	{
		if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
			return;


		if (!g::local_player && !g::local_player->IsAlive())
			return;

		float CurrentTime = g::local_player->m_nTickBase() * g::global_vars->interval_per_tick;

		for (int i = 0; i < indicator.size(); i++)
		{
			if (indicator[i].flEraseTime < CurrentTime)
			{
				indicator.erase(indicator.begin() + i);
				continue;
			}

			if (!indicator[i].bInitialized)
			{
				indicator[i].Position = indicator[i].Player->get_bone_position(8); //HITBOX_HEAD is returning some hitbox in belly,wtf??? 
				indicator[i].bInitialized = true;
			}

			if (CurrentTime - indicator[i].flLastUpdate > 0.001f) //was 0.0001f
			{
				indicator[i].Position.z -= (0.5f * (CurrentTime - indicator[i].flEraseTime)); //was 0.1f
				indicator[i].flLastUpdate = CurrentTime;
			}

			Vector ScreenPosition;

			Color color = Color::White;

			if (indicator[i].iDamage >= 100)
				color = Color::Red;

			if (indicator[i].iDamage >= 50 && indicator[i].iDamage < 100)
				color = Color::Orange;

			if (indicator[i].iDamage < 50)
				color = Color::White;

			if (math::world2screen(indicator[i].Position, ScreenPosition))
			{
				VGSHelper::Get().DrawTextW(std::to_string(indicator[i].iDamage).c_str(), ScreenPosition.x, ScreenPosition.y, color, 18);
			}
		}
	}

	void RenderPunchCross()
	{

		int w, h;

		g::engine_client->GetScreenSize(w, h);

		if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
			return;

		if (!g::local_player && !g::local_player->IsAlive())
			return;

		int x = w / 2;
		int y = h / 2;
		int dy = h / 97;
		int dx = w / 97; //97 or 85

		QAngle punchAngle = g::local_player->m_aimPunchAngle();
		x -= (dx * (punchAngle.yaw));
		y += (dy * (punchAngle.pitch));

		float radius = settings::visuals::radius;

		switch (settings::visuals::rcs_cross_mode)
		{
		case 0:
			globals::draw_list->AddLine(ImVec2(x - 5, y), ImVec2(x + 5, y), ImGui::GetColorU32(settings::visuals::recoilcolor));
			globals::draw_list->AddLine(ImVec2(x, y - 5), ImVec2(x, y + 5), ImGui::GetColorU32(settings::visuals::recoilcolor));
			break;
		case 1:
			globals::draw_list->AddCircle(ImVec2(x, y), radius, ImGui::GetColorU32(settings::visuals::recoilcolor), 255);
			break;
		}

	}

	void KnifeLeft()
	{

		static auto left_knife = g::cvar->find("cl_righthand");

		if (!g::local_player || !g::local_player->IsAlive())
		{
			left_knife->SetValue(1);
			return;
		}

		auto weapon = g::local_player->m_hActiveWeapon();
		if (!weapon) return;

		left_knife->SetValue(!weapon->IsKnife());
	}

	void Choke()
	{
		std::stringstream ss;
		ss << "choked: " << g::client_state->chokedcommands;

		if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
			return;

		if (!g::local_player && !g::local_player->IsAlive())
			return;

		VGSHelper::Get().DrawTextW(ss.str(), 10.0f, 450.0f, Color::White, 14);
	}

	void DrawFov() //todo
	{
		auto pWeapon = g::local_player->m_hActiveWeapon();
		if (!pWeapon)
			return;

		if (!g::engine_client->IsConnected() || !g::engine_client->IsInGame())
			return;

		auto settings = settings::aimbot::m_items[pWeapon->m_iItemDefinitionIndex()];

		bool dynamic_fov = settings.dynamic_fov;

		if (settings.enabled) {

			float fov = static_cast<float>(g::local_player->GetFOV());

			if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
				return;


			if (!g::local_player && !g::local_player->IsAlive())
				return;

			int w, h;
			g::engine_client->GetScreenSize(w, h);

			Vector2D screenSize = Vector2D(w, h);
			Vector2D center = screenSize * 0.5f;

			float ratio = screenSize.x / screenSize.y;
			float screenFov = atanf((ratio) * (0.75f) * tan(DEG2RAD(fov * 0.5f)));

			float radiusFOV;

			if (dynamic_fov)
			{
				Vector src3D, dst3D, forward;
				trace_t tr;
				Ray_t ray;
				CTraceFilter filter;

				QAngle angles = viewanglesBackup;
				math::angle2vectors(angles, forward);
				filter.pSkip = g::local_player;
				src3D = g::local_player->GetEyePos();
				dst3D = src3D + (forward * 8192);

				ray.Init(src3D, dst3D);
				g::engine_trace->trace_ray(ray, MASK_SHOT, &filter, &tr);

				QAngle leftViewAngles = QAngle(angles.pitch, angles.yaw - 90.f, 0.f);
				math::AngleNormalize(leftViewAngles);
				math::angle2vectors(leftViewAngles, forward);
				forward *= settings.fov * 7.f;

				Vector maxAimAt = tr.endpos + forward;

				Vector max2D;
				if (g::debug_overlay->ScreenPosition(maxAimAt, max2D))
					return;

				radiusFOV = fabsf(w / 2 - max2D.x);
			}
			else
			{
				radiusFOV = tanf(DEG2RAD(aimbot::get_fov())) / tanf(screenFov) * center.x;
			}

			//if (dynamic_fov && g::local_player->m_hActiveWeapon()->m_zoomLevel() == 1) //Single Scoped //No need to use now
				//screenFov = atanf((ratio) * (0.40f) * tan(DEG2RAD(fov * 1.0f)));

			//if (dynamic_fov && g::local_player->m_hActiveWeapon()->m_zoomLevel() == 2) //Double Scoped //No need to use now
				//screenFov = atanf((ratio) * (0.40f) * tan(DEG2RAD(fov * 1.0f)));

			globals::draw_list->AddCircle(ImVec2(center.x, center.y), radiusFOV, ImGui::GetColorU32(settings::visuals::drawfov_color), 255);
		}
	}

	void runCM(CUserCmd* cmd)
	{
		viewanglesBackup = cmd->viewangles;
	}


	void RenderHitmarker()
	{
		if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
			return;

		static int cx;
		static int cy;
		static int w, h;

		g::engine_client->GetScreenSize(w, h);
		cx = w / 2;
		cy = h / 2;

		//g_Saver.HitmarkerInfo.HitTime
		if (g::global_vars->realtime - saver.HitmarkerInfo.HitTime > .5f)
			return;

		float percent = (g::global_vars->realtime - saver.HitmarkerInfo.HitTime) / .5f;
		float percent2 = percent;

		if (percent > 1.f)
		{
			percent = 1.f;
			percent2 = 1.f;
		}

		percent = 1.f - percent;
		float addsize = percent2 * 5.f;

		ImVec4 clr = ImVec4{ 1.0f, 1.0f, 1.0f, percent * 1.0f };

		globals::draw_list->AddLine(ImVec2(cx - 3.f - addsize, cy - 3.f - addsize), ImVec2(cx + 3.f + addsize, cy + 3.f + addsize), ImGui::GetColorU32(clr));
		globals::draw_list->AddLine(ImVec2(cx - 3.f - addsize, cy + 3.f + addsize), ImVec2(cx + 3.f + addsize, cy - 3.f - addsize), ImGui::GetColorU32(clr));
	}

	void RenderNoScopeOverlay()
	{
		if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
			return;

		static int cx;
		static int cy;
		static int w, h;

		g::engine_client->GetScreenSize(w, h);
		cx = w / 2;
		cy = h / 2;


		if (g::local_player->m_bIsScoped())
		{
			globals::draw_list->AddLine(ImVec2(0, cy), ImVec2(w, cy), ImGui::GetColorU32(ImVec4{ 0.f, 0.f, 0.f, 1.0f }));
			globals::draw_list->AddLine(ImVec2(cx, 0), ImVec2(cx, h), ImGui::GetColorU32(ImVec4{ 0.f, 0.f, 0.f, 1.0f }));
			globals::draw_list->AddCircle(ImVec2(cx, cy), 255, ImGui::GetColorU32(ImVec4{0.f, 0.f, 0.f, 1.0f}), 255);
		}
	}

	void more_chams() noexcept
	{

		static IMaterial* mat = nullptr;

		static IMaterial* Metallic = g::mat_system->FindMaterial("simple_reflective", TEXTURE_GROUP_MODEL);

		mat = Metallic;

		for (int i = 0; i < interfaces::entity_list->GetHighestEntityIndex(); i++) {
			auto entity = reinterpret_cast<c_base_player*>(interfaces::entity_list->GetClientEntity(i));

			if (entity && entity != g::local_player) {
				auto client_class = entity->GetClientClass();
				auto model_name = interfaces::mdl_info->GetModelName(entity->GetModel());

				switch (client_class->m_ClassID) {
				case EClassId::CPlantedC4:
				case EClassId::CBaseAnimating:
					if (settings::chams::plantedc4_chams) {
						g::render_view->SetColorModulation(settings::chams::colorPlantedC4Chams.r() / 255.f, settings::chams::colorPlantedC4Chams.g() / 255.f, settings::chams::colorPlantedC4Chams.b() / 255.f);
						mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						interfaces::mdl_render->ForcedMaterialOverride(mat);
						mat->IncrementReferenceCount();
						entity->DrawModel(1, 255);
					}
					break;
				case EClassId::CHEGrenade:
				case EClassId::CFlashbang:
				case EClassId::CMolotovGrenade:
				case EClassId::CMolotovProjectile:
				case EClassId::CIncendiaryGrenade:
				case EClassId::CDecoyGrenade:
				case EClassId::CDecoyProjectile:
				case EClassId::CSmokeGrenade:
				case EClassId::CSmokeGrenadeProjectile:
				case EClassId::ParticleSmokeGrenade:
				case EClassId::CBaseCSGrenade:
				case EClassId::CBaseCSGrenadeProjectile:
				case EClassId::CBaseGrenade:
				case EClassId::CBaseParticleEntity:
				case EClassId::CSensorGrenade:
				case EClassId::CSensorGrenadeProjectile:
					if (settings::chams::nade_chams) {
						interfaces::render_view->SetColorModulation(settings::chams::colorNadeChams.r() / 255.f, settings::chams::colorNadeChams.g() / 255.f, settings::chams::colorNadeChams.b() / 255.f);
						mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						interfaces::mdl_render->ForcedMaterialOverride(mat);
						mat->IncrementReferenceCount();
						entity->DrawModel(1, 255);
					}
					break;
				}


				if (client_class->m_ClassID == CAK47 || client_class->m_ClassID == CDEagle || client_class->m_ClassID == CC4 ||
					client_class->m_ClassID >= CWeaponAug && client_class->m_ClassID <= CWeaponXM1014) {
					if (settings::chams::wep_droppedchams) {
						interfaces::render_view->SetColorModulation(settings::chams::ColorWeaponDroppedChams.r() / 255.f, settings::chams::ColorWeaponDroppedChams.g() / 255.f, settings::chams::ColorWeaponDroppedChams.b() / 255.f);
						mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						interfaces::mdl_render->ForcedMaterialOverride(mat);
						entity->DrawModel(1, 255);
					}
				}
				interfaces::mdl_render->ForcedMaterialOverride(nullptr);
				mat->IncrementReferenceCount();
			}
		}
	}

	void AAIndicator()
	{
		int x, y;

		g::engine_client->GetScreenSize(x, y);

		if (!g::local_player || !g::local_player->IsAlive())
			return;

		if (!utils::IsPlayingMM() && utils::IsValveDS())
			return;

		if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
			return;

		int cx = x / 2;
		int cy = y / 2;


		VGSHelper::Get().DrawText(side > 0.0f ? "<" : ">", side > 0.0f ? cx - 50 : cx + 40, cy - 10, Color::White, 19);
	}

	void DesyncChams()
	{
		if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
			return;

		if (interfaces::local_player->m_bGunGameImmunity() || interfaces::local_player->m_fFlags() & FL_FROZEN)
			return;

		if (!utils::IsPlayingMM() && utils::IsValveDS())
			return;
	
		Vector OrigAng;

		static IMaterial* material = nullptr;

		static IMaterial* Normal = g::mat_system->FindMaterial("simple_regular", TEXTURE_GROUP_MODEL);
		static IMaterial* Dogtags = g::mat_system->FindMaterial("models/inventory_items/dogtags/dogtags_outline", TEXTURE_GROUP_MODEL);
		static IMaterial* Flat = g::mat_system->FindMaterial("debug/debugdrawflat", TEXTURE_GROUP_MODEL);
		static IMaterial* Metallic = g::mat_system->FindMaterial("simple_reflective", TEXTURE_GROUP_MODEL);
		static IMaterial* Platinum = g::mat_system->FindMaterial("models/player/ct_fbi/ct_fbi_glass", TEXTURE_GROUP_MODEL);
		static IMaterial* Glass = g::mat_system->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_MODEL);
		static IMaterial* Crystal = g::mat_system->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_MODEL);
		static IMaterial* Gold = g::mat_system->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_MODEL);
		static IMaterial* DarkChrome = g::mat_system->FindMaterial("models/gibs/glass/glass", TEXTURE_GROUP_MODEL);
		static IMaterial* PlasticGloss = g::mat_system->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_MODEL);
		static IMaterial* Glow = g::mat_system->FindMaterial("vgui/achievements/glow", TEXTURE_GROUP_MODEL);

		Normal->IncrementReferenceCount();
		Dogtags->IncrementReferenceCount();
		Flat->IncrementReferenceCount();
		Metallic->IncrementReferenceCount();
		Platinum->IncrementReferenceCount();
		Glass->IncrementReferenceCount();
		Crystal->IncrementReferenceCount();
		Gold->IncrementReferenceCount();
		DarkChrome->IncrementReferenceCount();
		PlasticGloss->IncrementReferenceCount();
		Glow->IncrementReferenceCount();

		
		switch (settings::chams::desyncChamsMode)
		{
		case 0: material = Normal; 
			break;
		case 1: material = Dogtags; 
			break;
		case 2: material = Flat; 
			break;
		case 3: material = Metallic; 
			break;
		case 4: material = Platinum; 
			break;
		case 5: material = Glass; 
			break;
		case 6: material = Crystal;
			break;
		case 7: material = Gold;
			break;
		case 8: material = DarkChrome; 
			break;
		case 9: material = PlasticGloss; 
			break;
		case 10: material = Glow; 
			break;
		}

		OrigAng = g::local_player->GetAbsAngles2();
		g::local_player->SetAngle2(Vector(0, g::local_player->GetPlayerAnimState2()->m_flEyeYaw, 0)); //around 90% accurate
		if (g::Input->m_fCameraInThirdPerson)
		{
			g::mdl_render->ForcedMaterialOverride(material);
			g::render_view->SetColorModulation(1.0f, 1.0f, 1.0f);
		}
		g::local_player->GetClientRenderable()->DrawModel(0x1, 255);
		g::local_player->SetAngle2(OrigAng);
	}

	float csgo_armor(float damage, int armor_value) {
		float armor_ratio = 0.5f;
		float armor_bonus = 0.5f;
		if (armor_value > 0) {
			float armor_new = damage * armor_ratio;
			float armor = (damage - armor_new) * armor_bonus;

			if (armor > static_cast<float>(armor_value)) {
				armor = static_cast<float>(armor_value) * (1.f / armor_bonus);
				armor_new = damage - armor;
			}

			damage = armor_new;
		}
		return damage;
	}

	void bomb_esp(c_planted_c4* entity) noexcept {
		if (!settings::esp::bomb_esp)
			return;

		auto local_player = reinterpret_cast<c_base_player*>(interfaces::entity_list->GetClientEntity(interfaces::engine_client->GetLocalPlayer()));
		if (!local_player)
			return;

		auto explode_time = entity->m_flC4Blow();
		auto remaining_time = explode_time - (interfaces::global_vars->interval_per_tick * local_player->m_nTickBase());
		if (remaining_time < 0)
			return;

		int width, height;
		interfaces::engine_client->GetScreenSize(width, height);

		Vector bomb_origin, bomb_position;
		bomb_origin = entity->m_vecOrigin();

		explode_time -= interfaces::global_vars->interval_per_tick * local_player->m_nTickBase();
		if (explode_time <= 0)
			explode_time = 0;

		char buffer[64];
		sprintf_s(buffer, "%.2f", explode_time);

		auto c4_timer = interfaces::cvar->find("mp_c4timer")->GetInt();
		auto value = (explode_time * height) / c4_timer;

		//bomb damage indicator calculations, credits casual_hacker
		float damage;
		float hp_reimaing = g::local_player->m_iHealth();
		auto distance = local_player->GetEyePos().DistTo(entity->m_vecOrigin());
		auto a = 450.7f;
		auto b = 75.68f;
		auto c = 789.2f;
		auto d = ((distance - b) / c);
		auto fl_damage = a * exp(-d * d);
		damage = float((std::max)((int)ceilf(csgo_armor(fl_damage, g::local_player->m_ArmorValue())), 0));
		hp_reimaing -= damage;

		//convert damage to string
		//std::string damage_text;
		//damage_text += "-";
		//damage_text += std::to_string((int)(damage));
		//damage_text += "HP";

		std::string damage_text;
		damage_text += "HP LEFT: ";
		damage_text += std::to_string((int)(hp_reimaing));

		//render on screen bomb bar
		/*if (explode_time <= 10) {
			render.draw_filled_rect(0, 0, 10, value, color(255, 0, 0, 180));
		}
		else {
			render.draw_filled_rect(0, 0, 10, value, color(0, 255, 0, 180));
		} */

		c_planted_c4* bomb = nullptr;
		for (int i = 1; i < interfaces::entity_list->GetHighestEntityIndex(); i++) {

			if (entity->GetClientClass()->m_ClassID == EClassId::CPlantedC4) {
				bomb = (c_planted_c4*)entity;
				break;
			}
		}
		//render bomb timer
		//render.draw_text(12, value - 11, render.name_font_big, buffer, false, color(255, 255, 255));



		//render bomb damage
		if (g::local_player->IsAlive() && damage <= g::local_player->m_iHealth()) {
			VGSHelper::Get().DrawTextW(damage_text, width / 2 - 95, height / 2 + 255, Color::White, 50);
		}

		//render fatal check
		if (g::local_player->IsAlive() && damage >= g::local_player->m_iHealth()) {
			VGSHelper::Get().DrawTextW("HP LEFT: 0", width / 2 - 95, height / 2 + 255, Color::Red, 50);
			//VGSHelper::Get().DrawTextW("Fatal!", width / 2 - 50, height / 2 + 240, Color::Red, 50);
		}

		if (!math::world2screen(bomb_origin, bomb_position))
			return;
		VGSHelper::Get().DrawTextW(buffer, bomb_position.x - 13, bomb_position.y + 8, Color::White, 15);
		//VGSHelper::Get().DrawFilledBox(bomb_position.x - c4_timer / 2, bomb_position.y + 13, c4_timer, 3, Color::Black);  //wont draw 
		//VGSHelper::Get().DrawFilledBox(bomb_position.x - c4_timer / 2, bomb_position.y + 13, explode_time, 3, Color::Blue);
	}

	void SpreadCircle()
	{
		if (!g::local_player || !g::local_player->IsAlive())
			return;

		c_base_combat_weapon* weapon = g::local_player->m_hActiveWeapon().Get();

		if (!weapon)
			return;

		float spread = weapon->GetInaccuracy() * 1000;

		if (spread == 0.f)
			return;

		int x, y;
		g::engine_client->GetScreenSize(x, y);
		float cx = x / 2.f;
		float cy = y / 2.f;

		globals::draw_list->AddCircle(ImVec2(cx, cy), spread, ImGui::GetColorU32(settings::visuals::spread_cross_color), 255);
	}

	void glow() noexcept {

		auto local_player = reinterpret_cast<c_base_player*>(interfaces::entity_list->GetClientEntity(interfaces::engine_client->GetLocalPlayer()));
		if (!local_player)
			return;

		for (size_t i = 0; i < interfaces::glow_manager->size; i++) {
			auto& glow = interfaces::glow_manager->objects[i];
			if (glow.unused())
				continue;

			auto glow_entity = reinterpret_cast<c_base_player*>(glow.entity);
			auto client_class = glow_entity->GetClientClass();
			if (!glow_entity || glow_entity->IsDormant())
				continue;

			auto is_enemy = glow_entity->m_iTeamNum() != local_player->m_iTeamNum();
			auto is_teammate = glow_entity->m_iTeamNum() == local_player->m_iTeamNum();

			switch (client_class->m_ClassID) {
			case EClassId::CCSPlayer:
				if (is_enemy && settings::glow::glowEnemyEnabled) {
					glow.set(settings::glow::glowEnemyColor.r() / 255.f, settings::glow::glowEnemyColor.g() / 255.f, settings::glow::glowEnemyColor.b() / 255.f, settings::glow::glowEnemyColor.a() / 255.f);
				}
				else if (is_teammate && settings::glow::glowTeamEnabled) {
					glow.set(settings::glow::glowTeamColor.r() / 255.f, settings::glow::glowTeamColor.g() / 255.f, settings::glow::glowTeamColor.b() / 255.f, settings::glow::glowTeamColor.a() / 255.f);
				}
				break;
			case EClassId::CPlantedC4:
			case EClassId::CBaseAnimating:
				if (settings::glow::glowC4PlantedEnabled) {
					glow.set(settings::glow::glowC4PlantedColor.r() / 255.f, settings::glow::glowC4PlantedColor.g() / 255.f, settings::glow::glowC4PlantedColor.b() / 255.f, settings::glow::glowC4PlantedColor.a() / 255.f);
				}
				break;
			case EClassId::CHEGrenade:
			case EClassId::CFlashbang:
			case EClassId::CMolotovGrenade:
			case EClassId::CMolotovProjectile:
			case EClassId::CIncendiaryGrenade:
			case EClassId::CDecoyGrenade:
			case EClassId::CDecoyProjectile:
			case EClassId::CSmokeGrenade:
			case EClassId::CSmokeGrenadeProjectile:
			case EClassId::ParticleSmokeGrenade:
			case EClassId::CBaseCSGrenade:
			case EClassId::CBaseCSGrenadeProjectile:
			case EClassId::CBaseGrenade:
			case EClassId::CBaseParticleEntity:
			case EClassId::CSensorGrenade:
			case EClassId::CSensorGrenadeProjectile:
				if (settings::glow::glowNadesEnabled && !settings::glow::glowOverride) {
					glow.set(settings::glow::glowNadesColor.r() / 255.f, settings::glow::glowNadesColor.g() / 255.f, settings::glow::glowNadesColor.b() / 255.f, settings::glow::glowNadesColor.a() / 255.f);
				}
				break;
			}

			if (client_class->m_ClassID == CAK47 || client_class->m_ClassID == CDEagle || client_class->m_ClassID == CC4 ||
				client_class->m_ClassID >= CWeaponAug && client_class->m_ClassID <= CWeaponXM1014) {
				if (settings::glow::glowDroppedWeaponsEnabled) {
					glow.set(settings::glow::glowDroppedWeaponsColor.r() / 255.f, settings::glow::glowDroppedWeaponsColor.g() / 255.f, settings::glow::glowDroppedWeaponsColor.b() / 255.f, settings::glow::glowDroppedWeaponsColor.a() / 255.f);
				}
			}
		}
	}

	void glow_override() noexcept {

		auto local_player = reinterpret_cast<c_base_player*>(interfaces::entity_list->GetClientEntity(interfaces::engine_client->GetLocalPlayer()));
		if (!local_player)
			return;

		for (size_t i = 0; i < interfaces::glow_manager->size; i++) {
			auto& glow = interfaces::glow_manager->objects[i];
			if (glow.unused())
				continue;

			auto glow_entity = reinterpret_cast<c_base_player*>(glow.entity);
			auto client_class = glow_entity->GetClientClass();
			if (!glow_entity || glow_entity->IsDormant())
				continue;

			auto is_enemy = glow_entity->m_iTeamNum() != local_player->m_iTeamNum();
			auto is_teammate = glow_entity->m_iTeamNum() == local_player->m_iTeamNum();


			switch (client_class->m_ClassID) {
			
			case EClassId::CHEGrenade:
				if (settings::glow::glowOverride) {
					glow.set(settings::glow::glowHE.r() / 255.f, settings::glow::glowHE.g() / 255.f, settings::glow::glowHE.b() / 255.f, settings::glow::glowHE.a() / 255.f);
				}
				break;
			case EClassId::CFlashbang:
				if (settings::glow::glowOverride) {
					glow.set(settings::glow::glowFlashbang.r() / 255.f, settings::glow::glowFlashbang.g() / 255.f, settings::glow::glowFlashbang.b() / 255.f, settings::glow::glowFlashbang.a() / 255.f);
				}
				break;
			case EClassId::CMolotovGrenade:
				if (settings::glow::glowOverride) {
					glow.set(settings::glow::glowMolotovIncendiary.r() / 255.f, settings::glow::glowMolotovIncendiary.g() / 255.f, settings::glow::glowMolotovIncendiary.b() / 255.f, settings::glow::glowMolotovIncendiary.a() / 255.f);
				}
				break;
			case EClassId::CMolotovProjectile:
				if (settings::glow::glowOverride) {
					glow.set(settings::glow::glowMolotovIncendiary.r() / 255.f, settings::glow::glowMolotovIncendiary.g() / 255.f, settings::glow::glowMolotovIncendiary.b() / 255.f, settings::glow::glowMolotovIncendiary.a() / 255.f);
				}
				break;
			case EClassId::CIncendiaryGrenade:
				if (settings::glow::glowOverride) {
					glow.set(settings::glow::glowMolotovIncendiary.r() / 255.f, settings::glow::glowMolotovIncendiary.g() / 255.f, settings::glow::glowMolotovIncendiary.b() / 255.f, settings::glow::glowMolotovIncendiary.a() / 255.f);
				}
				break;
			case EClassId::CDecoyGrenade:
				if (settings::glow::glowOverride) {
					glow.set(settings::glow::glowDecoy.r() / 255.f, settings::glow::glowDecoy.g() / 255.f, settings::glow::glowDecoy.b() / 255.f, settings::glow::glowDecoy.a() / 255.f);
				}
				break;
			case EClassId::CDecoyProjectile:
				if (settings::glow::glowOverride) {
					glow.set(settings::glow::glowDecoy.r() / 255.f, settings::glow::glowDecoy.g() / 255.f, settings::glow::glowDecoy.b() / 255.f, settings::glow::glowDecoy.a() / 255.f);
				}
				break;
			case EClassId::CSmokeGrenade:
				if (settings::glow::glowOverride) {
					glow.set(settings::glow::glowSmoke.r() / 255.f, settings::glow::glowSmoke.g() / 255.f, settings::glow::glowSmoke.b() / 255.f, settings::glow::glowSmoke.a() / 255.f);
				}
				break;
			case EClassId::CSmokeGrenadeProjectile:
				if (settings::glow::glowOverride) {
					glow.set(settings::glow::glowSmoke.r() / 255.f, settings::glow::glowSmoke.g() / 255.f, settings::glow::glowSmoke.b() / 255.f, settings::glow::glowSmoke.a() / 255.f);
				}
				break;
			}

			if (settings::glow::glowOverride) 
			{
				if (client_class->m_ClassID == CC4)
				{
					glow.set(settings::glow::glowDroppedC4Color.r() / 255.f, settings::glow::glowDroppedC4Color.g() / 255.f, settings::glow::glowDroppedC4Color.b() / 255.f, settings::glow::glowDroppedC4Color.a() / 255.f);
				}
			}
		}
	}

	void fetch_entities()
	{
		render_mutex.lock();

		entities.clear();

		if (!is_enabled())
		{
			render_mutex.unlock();
			return;
		}

		for (auto i = 1; i <= interfaces::entity_list->GetHighestEntityIndex(); ++i)
		{
			auto* entity = c_base_player::GetPlayerByIndex(i);

			if (!entity || entity->IsPlayer() || entity->is_dormant() || entity == interfaces::local_player)
				continue;

			const auto classid = entity->GetClientClass()->m_ClassID;
			if (settings::visuals::world_grenades && (classid == 9 || classid == 134 || classid == 111 || classid == 113 || classid == 156 || classid == 48)) //9 = HE,113 = molly,156 = smoke,48 = decoy
				world_grenades(entity);
			else if (settings::visuals::planted_c4 && entity->IsPlantedC4())
				push_entity(entity, "Bomb", Color::Yellow);
			else if (settings::visuals::defuse_kit && entity->IsDefuseKit() && !entity->m_hOwnerEntity().IsValid())
				push_entity(entity, "Defuse Kit", Color::Green);
			else if (settings::visuals::dropped_weapons && entity->IsWeapon() && !entity->m_hOwnerEntity().IsValid())
				push_entity(entity, utils::get_weapon_name(entity), Color::White);
		}

		render_mutex.unlock();
	}

	void render(ImDrawList* draw_list)
	{
		if (!is_enabled() || !render::fonts::visuals)
			return;

		if (render_mutex.try_lock())
		{
			saved_entities = entities;
			render_mutex.unlock();
		}

		ImGui::PushFont(render::fonts::visuals);

		Vector origin;
		for (const auto& entity : saved_entities)
		{
			if (math::world2screen(entity.origin, origin))
			{
				const auto text_size = ImGui::CalcTextSize(entity.text.c_str());
				imdraw::outlined_text(entity.text.c_str(), ImVec2(origin.x - text_size.x / 2.f, origin.y), utils::to_im32(entity.color));
			}
		}

		if(settings::visuals::rcs_cross)
			RenderPunchCross();

		if (settings::esp::drawFov)
			DrawFov();

		if (settings::visuals::hitmarker)
			RenderHitmarker();

		if (settings::misc::noscope)
			RenderNoScopeOverlay();

		if (settings::visuals::spread_cross)
			SpreadCircle();


		ImGui::PopFont();
	}
}