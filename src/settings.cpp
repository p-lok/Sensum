#include "settings.h"
#include "config.h"
#include "options.hpp"
#include "jsoncpp/json.h"
#include "helpers/imdraw.h"

namespace settings
{
	namespace movement
	{
		bool engine_prediction = false;
	}

	namespace esp
	{
		bool enabled = false;
		bool visible_only = false;
		bool boxes = false;
		int box_type = 0;
		bool names = false;
		bool health = false;
		int health_position = 0;
		bool armour = false;
		int armour_position = 1;
		bool weapons = false;
		int weapon_mode = 0;
		bool ammo = false;
		bool offscreen = false;
		bool sound = false;
		bool snaplines = false;
		bool is_scoped = false;
		bool dormant = false;
		bool beams = false;
		int esp_on_chance;
		int esp_off_chance;
		int esp_chance;
		bool is_flashed = false;
		bool is_defusing = false;
		bool is_reloading = false;
		float mfts = 0.2f;
		bool mat_force = false;
		bool drawFov;
		bool bomb_esp = false;
		bool kevlarinfo = false;
		bool haskit = false;
		bool money = false;
		bool antiobs = false;
		bool soundesp = false;
		bool mat_force_apply = false;
		
		Color visibleColor = Color(0, 200, 80);
		Color occludedColor = Color(0, 0, 0);
		Color colorSoundEsp = Color(154, 0, 255, 255);
	}

	namespace glow
	{
		bool glowEnemyEnabled = false;
		bool glowTeamEnabled = false;
		bool glowC4PlantedEnabled = false;
		bool glowDroppedWeaponsEnabled = false;
		bool glowNadesEnabled = false;
		bool glowOverride = false;

		Color glowEnemyColor = Color(255, 0, 0, 255);
		Color glowTeamColor(0, 255, 0, 255);
		Color glowC4PlantedColor = Color(255, 255, 255, 255);
		Color glowDroppedWeaponsColor = Color(255, 255, 255, 255);
		Color glowNadesColor = Color(255, 255, 255, 255);
		Color glowHE = Color(255, 0, 0, 255);
		Color glowMolotovIncendiary = Color(255, 128, 0, 255);
		Color glowFlashbang = Color(255, 255, 0);
		Color glowDecoy = Color(0, 255, 0, 255);
		Color glowSmoke = Color(105, 105, 105, 255);
		Color glowDroppedC4Color = Color(0, 255, 0, 255);

	}

	namespace chams
	{
		bool enabled = false;
		bool visible_only = true;
		bool wireframe = false;
		bool flat = false;
		bool desync = false;
		bool localnew = false;
		bool teamnew = false;
		bool enemynew = false;
		bool btchams = false;
		bool btflat = false;
		bool xqz = false;
		int bttype = 0;
		bool wepchams = false;
		bool wep_droppedchams = false;
		bool wep_droppedchams_xqz = false;
		bool nade_chams = false;
		bool nade_chams_xqz = false;
		bool plantedc4_chams = false;
		bool plantedc4_chams_xqz = false;
		int matmode;
		int localmodenew = 0;
		int enemymodenew = 0;
		int teammodenew = 0;
		int desyncChamsMode = 0;

		Color btColorChams = Color(0, 255, 248, 147);
		Color ColorWeaponDroppedChams = Color(0, 255, 125, 255);
		Color colorNadeChams = Color(0, 255, 125, 255);
		Color colorPlantedC4Chams = Color(0, 255, 125, 255);
		Color visible_color = Color(0, 200, 80);
		Color occluded_color = Color(0, 0, 0);
		Color LocalColor_vis = Color(0, 200, 80);
		Color TeamColor_vis = Color(0, 200, 80);
		Color EnemyColor_vis = Color(0, 200, 0);
		Color LocalColor_XQZ = Color(0, 200, 0);
		Color TeamColor_XQZ = Color(0, 200, 0);
		Color EnemyColor_XQZ = Color(0, 200, 0);
		Color ChamsPlantedC4XQZ = Color(0, 255, 0, 255);
		Color ChamsHEXQZ = Color(255, 0, 0, 255);
		Color ChamsMolotovIncendiaryXQZ = Color(255, 128, 0, 255);
		Color ChamsFlashbangXQZ = Color(255, 255, 0);
		Color ChamsDecoyXQZ = Color(0, 255, 0, 255);
		Color ChamsSmokeXQZ = Color(105, 105, 105, 255);
		Color ChamsC4Dropped = Color(0, 255, 255, 255);

		namespace arms
		{
			bool enabled = false;
			bool wireframe = true;

			ImVec4 color = ImVec4::Red;
		}
	}

	namespace visuals
	{
		bool grenade_prediction = false;
		bool player_info_box = false;
		bool grief_box = false;
		bool world_grenades = false;
		bool sniper_crosshair = true;
		bool defuse_kit = false;
		bool planted_c4 = false;
		bool dropped_weapons = false;
		bool night_mode = false;
		bool pitch = false;
		bool hitmarker = false;
		int hitsound;
		bool skychanger = false;
		int skychanger_mode = 0;
		bool newstyle = false;
		bool rcs_cross = false;
		bool bullet_tracer = false;
		float radius = 12;
		int rcs_cross_mode;
		bool spread_cross = false;
		bool a = false;
		bool b = false;
		bool choke = false;
		bool skeletons = false;
		float player_info_box_alpha = 1.0f;
		float grief_box_alpha = 1.0f;
		int viewmodel_mode = 0;

		ImVec4 sky = ImVec4(0.25, 0.25, 0.25, 1.0);
		ImVec4 recoilcolor = ImVec4{0.f, 0.f, 0.f, 1.0f};
		ImVec4 spread_cross_color = ImVec4{1.f, 0.f, 0.f, 1.0f};
		ImVec4 drawfov_color = ImVec4{1.f, 1.f, 1.f, 1.0f};

		Color colorBulletTracer = Color(154, 0, 255, 255);
	}

	namespace desync
	{
		bool enabled = false;
		bool yaw_flip = false;
		bool resolver = false;
		int desync_mode = 0;
		bool enabled2 = false;
	}

	namespace aimbot
	{
		int setting_type = settings_type_t::separately;

		std::vector<aimbot_group> m_groups = { };
		std::map<int, weapon_setting> m_items = { };
	}

	namespace fake_lags
	{
		bool enabled = false;
		int type = fake_lag_types::lag_always;
		int factor = 12;
	}

	namespace lightning_shot
	{
		bool enabled = false;
		int delay = 4;
		bool check_zoom = true;
	}

	namespace misc
	{
		bool radar = false;
		bool disable_on_weapon = false;
		bool bhop = false;
		bool auto_strafe = false;
		bool no_smoke = false;
		bool no_flash = false;
		bool human_bhop = false;
		float viewmodel_fov = 68.f;
		bool moon_walk = false;
		bool deathmatch = false;
		bool knife_bot = false;
		int bhop_hit_chance;
		int hops_restricted_limit;
		int max_hops_hit;
		int debug_fov = 90;
		bool esp_random;
		bool lknife;
		bool noscope;
		bool selfnade;
		bool lefthandknife;
		bool damage_indicator = false;
		bool fast_stop = false;
		bool override_viewmodel = false;

		float viewmodel_offset_x = 0.f;
		float viewmodel_offset_y = 0.f;
		float viewmodel_offset_z = 0.f;
	};

	void load(std::string name)
	{
		config::load(name, "settings", true, [](Json::Value root)
		{
			Option::Load(root["esp.enabled"], esp::enabled);
			Option::Load(root["esp.visible_only"], esp::visible_only);
			Option::Load(root["esp.boxes"], esp::boxes, true);
			Option::Load(root["esp.names"], esp::names);
			Option::Load(root["esp.box_type"], esp::box_type, EBoxType::Normal);
			Option::Load(root["esp.health"], esp::health);
			Option::Load(root["esp.health_position"], esp::health_position, 0);
			Option::Load(root["esp.armour"], esp::armour);
			Option::Load(root["esp.armour_position"], esp::armour_position, 1);
			Option::Load(root["esp.weapons"], esp::weapons);
			Option::Load(root["esp.offscreen"], esp::offscreen);
			Option::Load(root["esp.sound"], esp::sound);
			Option::Load(root["esp.dormant"], esp::dormant);
			Option::Load(root["esp.is_scoped"], esp::is_scoped);
			Option::Load(root["esp.snaplines"], esp::snaplines);
			Option::Load(root["esp.beams"], esp::beams);
			Option::Load(root["esp.random"], misc::esp_random);
			Option::Load(root["esp.esp_on_chance"], esp::esp_on_chance);
			Option::Load(root["esp.esp_off_chance"], esp::esp_off_chance);
			Option::Load(root["esp.is_flashed"], esp::is_flashed);
			Option::Load(root["esp.is_defusing"], esp::is_defusing);
			Option::Load(root["esp.mat_force_enabled"], esp::mat_force);
			Option::Load(root["esp.mat_force_value"], esp::mfts);
			Option::Load(root["esp.draw_aimbot_fov"], esp::drawFov);
			Option::Load(root["esp.chance_value"], settings::esp::esp_chance);
			Option::Load(root["esp.sound_esp"], settings::esp::soundesp);
			Option::Load(root["esp.draw_aimbot_fov_color"], settings::visuals::drawfov_color);
			Option::Load(root["esp.damage_indicator"], settings::misc::damage_indicator);
			Option::Load(root["esp_weapon_mode"], settings::esp::weapon_mode);
			Option::Load(root["esp.player_info_box_alpha"], settings::visuals::player_info_box_alpha);
			Option::Load(root["esp.grief_box_alpha"], settings::visuals::grief_box_alpha);
			Option::Load(root["esp.visible_color"], esp::visibleColor, Color(0, 200, 80));
			Option::Load(root["esp.occluded_color"], esp::occludedColor, Color::Black);

			Option::Load(root["visuals.world_grenades"], visuals::world_grenades, true);
			Option::Load(root["visuals.sniper_crosshair"], visuals::sniper_crosshair, true);
			Option::Load(root["visuals.grenade_prediction"], visuals::grenade_prediction, true);
			Option::Load(root["visuals.defuse_kit"], visuals::defuse_kit);
			Option::Load(root["visuals.planted_c4"], visuals::planted_c4);
			Option::Load(root["visuals.dropped_weapons"], visuals::dropped_weapons);
			Option::Load(root["visuals.sky"], visuals::sky, ImVec4(0.25, 0.25, 0.25, 1.0));
			Option::Load(root["visuals.night_mode"], visuals::night_mode);

			imdraw::apply_style(visuals::night_mode);

			Option::Load(root["chams.visible_only"], chams::visible_only, true);
			Option::Load(root["chams.flat"], chams::flat, false);
			Option::Load(root["chams.wireframe"], chams::wireframe, false);
			Option::Load(root["chams.enemy"], chams::enemynew);
			Option::Load(root["chams.team"], chams::teamnew);
			Option::Load(root["chams.local"], chams::localnew);
			Option::Load(root["chams.enemy_mode"], chams::enemymodenew);
			Option::Load(root["chams.team_mode"], chams::teammodenew);
			Option::Load(root["chams.local_mode"], chams::localmodenew);
			Option::Load(root["chams.enemy_color_vis"], chams::EnemyColor_vis, Color::Black);
			Option::Load(root["chams.team_color_vis"], chams::TeamColor_vis, Color::Black);
			Option::Load(root["chams.local_color_vis"], chams::LocalColor_vis, Color::Black);
			Option::Load(root["chams.enemy_color_xqz"], chams::EnemyColor_XQZ, Color(0, 200, 80));
			Option::Load(root["chams.team_color_xqz"], chams::TeamColor_XQZ, Color(0, 200, 80));
			Option::Load(root["chams.local_color_xqz"], chams::LocalColor_XQZ, Color(0, 200, 80));
			Option::Load(root["chams.legit_aa"], chams::desync);
			Option::Load(root["chams.backtrack"], chams::bttype);
			Option::Load(root["chams.backtrack_flat"], chams::btflat);
			Option::Load(root["chams.backtrack_color"], chams::btColorChams);
			Option::Load(root["chams.viewmodel_weapons"], chams::wepchams);
			Option::Load(root["chams.dropped_weapons"], chams::wep_droppedchams);
			Option::Load(root["chams.dropped_weapons_color"], chams::ColorWeaponDroppedChams);
			Option::Load(root["chams.plantedc4"], chams::plantedc4_chams);
			Option::Load(root["chams.plantedc4_color"], chams::colorPlantedC4Chams);
			Option::Load(root["chams.nades"], chams::nade_chams);
			Option::Load(root["chams.nades_color"], chams::colorNadeChams);
			Option::Load(root["chams.arms.enabled"], chams::arms::enabled, false);
			Option::Load(root["chams.arms.wireframe"], chams::arms::wireframe, true);
			Option::Load(root["chams.arms.color"], chams::arms::color, ImVec4::Red);
			Option::Load(root["chams.visible_color"], chams::visible_color, Color(0, 200, 80));
			Option::Load(root["chams.occluded_color"], chams::occluded_color, Color::Black);
			Option::Load(root["chams.plantedc4_color"], settings::chams::ChamsPlantedC4XQZ);
			Option::Load(root["chams.he_color"], settings::chams::ChamsHEXQZ);
			Option::Load(root["chams.molotov_inc_color"], settings::chams::ChamsMolotovIncendiaryXQZ);
			Option::Load(root["chams.flashbang_color"], settings::chams::ChamsFlashbangXQZ);
			Option::Load(root["chams.decoy_color"], settings::chams::ChamsDecoyXQZ);
			Option::Load(root["chams.smoke_color"], settings::chams::ChamsSmokeXQZ);
			Option::Load(root["chams.C4dropped_color"], settings::chams::ChamsC4Dropped);
			
			Option::Load(root["misc.bhop"], misc::bhop);
			Option::Load(root["misc.auto_strafe"], misc::auto_strafe);
			Option::Load(root["misc.viewmodel_fov"], misc::viewmodel_fov, 68);
			Option::Load(root["misc.moon_walk"], misc::moon_walk);
			Option::Load(root["misc.deathmatch"], misc::deathmatch);
			Option::Load(root["misc.no_flash"], misc::no_flash);
			Option::Load(root["misc.no_smoke"], misc::no_smoke);
			Option::Load(root["misc.knife_bot"], misc::knife_bot);
			Option::Load(root["misc.radar"], misc::radar);
			Option::Load(root["misc.human_bhop"], misc::human_bhop);
			Option::Load(root["misc.human_bhop_hopchance"], misc::bhop_hit_chance);
			Option::Load(root["misc.human_bhop_hopslimit"], misc::hops_restricted_limit);
			Option::Load(root["misc.human_bhop_maxhops_hit"], misc::max_hops_hit);
			Option::Load(root["misc.resolver"], desync::resolver);
			Option::Load(root["misc.engine_prediction"], movement::engine_prediction);
			Option::Load(root["misc.knife_bot"], misc::knife_bot);
			Option::Load(root["misc.debug_fov"], misc::debug_fov);
			Option::Load(root["misc.skychanger"], visuals::skychanger);
			Option::Load(root["misc.skychanger_mode"], visuals::skychanger_mode);
			Option::Load(root["misc.menu_style"], visuals::newstyle);
			Option::Load(root["misc.rcs_cross"], visuals::rcs_cross);
			Option::Load(root["misc.rcs_cross_mode"], visuals::rcs_cross_mode);
			Option::Load(root["misc.bullet_tracer"], visuals::bullet_tracer, false);
			Option::Load(root["misc.bullet_tracer_color"], visuals::colorBulletTracer, Color(154, 0, 255, 255)); //edited here
			Option::Load(root["misc.radius"], visuals::radius, 12.f);
			Option::Load(root["mics.rcs_cross_color"], visuals::recoilcolor);
			Option::Load(root["misc.no_scope_overlay"], misc::noscope);
			Option::Load(root["misc.self_nade"], misc::selfnade);
			Option::Load(root["misc.hitmarker"], visuals::hitmarker);
			Option::Load(root["misc.hitmarker_sound"], visuals::hitsound);
			Option::Load(root["misc.bomb_damage_esp"], esp::bomb_esp);
			Option::Load(root["misc_kevlar_info"], settings::esp::kevlarinfo);
			Option::Load(root["misc_spread_crosshair"], settings::visuals::spread_cross);
			Option::Load(root["misc_spread_crosshair_color"], settings::visuals::spread_cross_color);
			Option::Load(root["misc_esp_money"], settings::esp::money);
			Option::Load(root["misc_esp_ammo"], settings::esp::ammo);
			Option::Load(root["misc_esp_kit"], settings::esp::haskit);
			Option::Load(root["misc.choke_indicator"], settings::visuals::choke);
			Option::Load(root["misc.player_info_box"], settings::visuals::player_info_box);
			Option::Load(root["misc.grief_box"], settings::visuals::grief_box);
			Option::Load(root["misc.disable_3rd_person_on_weapon"], settings::misc::disable_on_weapon);
			Option::Load(root["misc.left_hand_knife"], settings::misc::lefthandknife);
			Option::Load(root["misc.fast_stop"], settings::misc::fast_stop);
			Option::Load(root["misc.viewmodel_mode"], settings::visuals::viewmodel_mode);
			Option::Load(root["misc.viewmodel_override"], settings::misc::override_viewmodel);
			Option::Load(root["misc.viewmodel_offset_x"], settings::misc::viewmodel_offset_x);
			Option::Load(root["misc.viewmodel_offset_y"], settings::misc::viewmodel_offset_y);
			Option::Load(root["misc.viewmodel_offset_z"], settings::misc::viewmodel_offset_z);

			Option::Load(root["glow.enemy"], glow::glowEnemyEnabled);
			Option::Load(root["glow.team"], glow::glowTeamEnabled);
			Option::Load(root["glow.plantedc4"], glow::glowC4PlantedEnabled);
			Option::Load(root["glow.nades"], glow::glowNadesEnabled);
			Option::Load(root["glow.dropped_weapons"], glow::glowDroppedWeaponsEnabled);
			Option::Load(root["glow.override"], glow::glowOverride);

			Option::Load(root["glow.enemy_color"], glow::glowEnemyColor);
			Option::Load(root["glow.team_color"], glow::glowTeamColor);
			Option::Load(root["glow.plantedc4_color"], glow::glowC4PlantedColor);
			Option::Load(root["glow.nades_color"], glow::glowNadesColor);
			Option::Load(root["glow.dropped_weapons_color"], glow::glowDroppedWeaponsColor);

			Option::Load(root["fake_lags.enabled"], fake_lags::enabled);
			Option::Load(root["fake_lags.factor"], fake_lags::factor, 6);
			Option::Load(root["fake_lags.type"], fake_lags::type);

			Option::Load(root["desync.enabled"], desync::enabled);
			Option::Load(root["desync.yaw_flip"], desync::yaw_flip);
			Option::Load(root["desync.resolver"], desync::resolver);
			Option::Load(root["desync.enabled2"], desync::enabled2);
			Option::Load(root["desync_mode"], desync::desync_mode);
			Option::Load(root["desync.chams_mode"], desync::desync_mode);

			Option::Load(root["legitbot.setting_type"], aimbot::setting_type);

			aimbot::m_items.clear();

			Json::Value aimbot_items = root["legitbot.items"];
			for (Json::Value::iterator it = aimbot_items.begin(); it != aimbot_items.end(); ++it)
			{
				Json::Value settings = *it;

				weapon_setting data;
				Option::Load(settings["enabled"], data.enabled);
				Option::Load(settings["back_shot"], data.back_shot);
				Option::Load(settings["autodelay"], data.autodelay);
				Option::Load(settings["autopistol"], data.autopistol);
				Option::Load(settings["check_smoke"], data.check_smoke);
				Option::Load(settings["check_flash"], data.check_flash);
				Option::Load(settings["check_air"], data.check_air, true);
				Option::Load(settings["backtrack.enabled"], data.backtrack.enabled);
				Option::Load(settings["backtrack.legit"], data.backtrack.legit);
				Option::Load(settings["backtrack.ticks"], data.backtrack.ticks, 6);
				Option::Load(settings["backtrack.time"], data.backtrack.time, 200);
				if (data.backtrack.ticks < 0 || data.backtrack.ticks > 12)
					data.backtrack.ticks = 6;

				if (data.backtrack.time < 0 || data.backtrack.time > 200)
					data.backtrack.time = 200;

				Option::Load(settings["check_zoom"], data.check_zoom, true);
				Option::Load(settings["silent.enabled"], data.silent.enabled, false);
				Option::Load(settings["silent.fov"], data.silent.fov, 0.f);
				Option::Load(settings["silent.always"], data.silent.always, false);
				Option::Load(settings["silent.chanse"], data.silent.chanse, 7);
				if (data.silent.chanse > 10)
					data.silent.chanse = std::clamp<int>(data.silent.chanse / 10, 0, 10);

				Option::Load(settings["silent.with_smooth"], data.silent.with_smooth, true);

				Option::Load(settings["trigger.enabled"], data.trigger.enabled);
				Option::Load(settings["trigger.in_radius"], data.trigger.in_radius);
				Option::Load(settings["trigger.delay"], data.trigger.delay);
				Option::Load(settings["trigger.delay_btw_shots"], data.trigger.delay_btw_shots, 50);
				Option::Load(settings["trigger.hitchance"], data.trigger.hitchance, 1);

				Option::Load(settings["hitboxes.head"], data.hitboxes.head, true);
				Option::Load(settings["hitboxes.neck"], data.hitboxes.neck, true);
				Option::Load(settings["hitboxes.body"], data.hitboxes.body, true);
				Option::Load(settings["hitboxes.hands"], data.hitboxes.hands);
				Option::Load(settings["hitboxes.legs"], data.hitboxes.legs);

				Option::Load(settings["dynamic_fov"], data.dynamic_fov);
				Option::Load(settings["fov"], data.fov);
				Option::Load(settings["smooth"], data.smooth, 1);
				Option::Load(settings["shot_delay"], data.shot_delay);
				Option::Load(settings["kill_delay"], data.kill_delay, 600);
				Option::Load(settings["by_damage"], data.by_damage);
				Option::Load(settings["min_damage"], data.min_damage, 0);
				Option::Load(settings["hitchanse"], data.min_hitchanse, 0);
				Option::Load(settings["rcs_hitbox_override"], data.rcs_override_hitbox);

				Option::Load(settings["autowall.enabled"], data.autowall.enabled);
				Option::Load(settings["autowall.min_damage"], data.autowall.min_damage, 1);

				Option::Load(settings["rcs"], data.recoil.enabled);
				Option::Load(settings["rcs.humanize"], data.recoil.humanize);
				Option::Load(settings["rcs.first_bullet"], data.recoil.first_bullet);
				Option::Load(settings["rcs.standalone"], data.recoil.standalone, true);
				Option::Load(settings["rcs_value_x"], data.recoil.pitch, 2.f);
				Option::Load(settings["rcs_value_y"], data.recoil.yaw, 2.f);

				aimbot::m_items[it.key().asInt()] = data;
			}

			settings::aimbot::m_groups.clear();

			for (auto group : root["aimbot.groups"])
			{
				std::vector<int> weapons = { };

				for (auto& weapon_index : group["weapons"])
				{
					weapons.emplace_back(weapon_index.asInt());
				}

				settings::aimbot::m_groups.emplace_back(aimbot_group{ group["name"].asString(), weapons });
			}
		});
	}

	void save(std::string name)
	{
		config::save(name, "settings", true, []()
		{
			Json::Value config;

			config["esp.enabled"] = esp::enabled;
			config["esp.visible_only"] = esp::visible_only;
			config["esp.boxes"] = esp::boxes;
			config["esp.names"] = esp::names;
			config["esp.box_type"] = esp::box_type;
			config["esp.health"] = esp::health;
			config["esp.health_position"] = esp::health_position;
			config["esp.armour"] = esp::armour;
			config["esp.armour_position"] = esp::armour_position;
			config["esp.weapons"] = esp::weapons;
			config["esp.offscreen"] = esp::offscreen;
			config["esp.sound"] = esp::sound;
			config["esp.dormant"] = esp::dormant;
			config["esp.is_scoped"] = esp::is_scoped;
			config["esp.snaplines"] = esp::snaplines;
			config["esp.beams"] = esp::beams;
			config["esp.random"] = misc::esp_random;
			config["esp.esp_on_chance"] = esp::esp_on_chance;
			config["esp.esp_off_chance"] = esp::esp_off_chance;
			config["esp.is_defusing"] = esp::is_defusing;
			config["esp.is_flashed"] = esp::is_flashed;
			config["esp.random"] = misc::esp_random;
			config["esp.mat_force_enabled"] = esp::mat_force;
			config["esp.mat_force_value"] = esp::mfts;
			config["esp.draw_aimbot_fov"] = esp::drawFov;
			config["esp.chance_value"] = settings::esp::esp_chance;
			config["esp.sound_esp"] = settings::esp::soundesp;
			config["esp.damage_indicator"] = settings::misc::damage_indicator;
			config["esp_weapon_mode"] = settings::esp::weapon_mode;
			config["esp.player_info_box_alpha"] = settings::visuals::player_info_box_alpha;
			config["esp.grief_box_alpha"] = settings::visuals::grief_box_alpha;

			Option::Save(config["esp.visible_color"], esp::visibleColor);
			Option::Save(config["esp.occluded_color"], esp::occludedColor);
			Option::Save(config["esp.draw_aimbot_fov_color"], settings::visuals::drawfov_color);
			Option::Save(config["visuals.sky"], visuals::sky);

			config["visuals.world_grenades"] = visuals::world_grenades;
			config["visuals.sniper_crosshair"] = visuals::sniper_crosshair;
			config["visuals.grenade_prediction"] = visuals::grenade_prediction;
			config["visuals.defuse_kit"] = visuals::defuse_kit;
			config["visuals.planted_c4"] = visuals::planted_c4;
			config["visuals.dropped_weapons"] = visuals::dropped_weapons;
			config["visuals.night_mode"] = visuals::night_mode;

			config["chams.visible_only"] = chams::visible_only;
			config["chams.flat"] = chams::flat;
			config["chams.wireframe"] = chams::wireframe;
			config["chams.enemy_mode"] = chams::enemymodenew;
			config["chams.team_mode"] = chams::teammodenew;
			config["chams.local_mode"] = chams::localmodenew;
			config["chams.enemy"] = chams::enemynew;
			config["chams.team"] = chams::teamnew;
			config["chams.local"] = chams::localnew;
			config["chams.legit_aa"] = chams::desync;
			config["chams.backtrack"] = chams::bttype;
			config["chams.backtrack_flat"] = chams::btflat;
			config["chams.dropped_weapons"] = chams::wep_droppedchams;
			config["chams.plantedc4"] = chams::plantedc4_chams;
			config["chams.nades"] = chams::nade_chams;
			config["chams.arms.enabled"] = chams::arms::enabled;
			config["chams.arms.wireframe"] = chams::arms::wireframe;

			Option::Save(config["chams.backtrack_color"], chams::btColorChams);
			Option::Save(config["chams.enemy_color_vis"], chams::EnemyColor_vis);
			Option::Save(config["chams.team_color_vis"], chams::TeamColor_vis);
			Option::Save(config["chams.local_color_vis"], chams::LocalColor_vis);
			Option::Save(config["chams.enemy_color_xqz"], chams::EnemyColor_XQZ);
			Option::Save(config["chams.team_color_xqz"], chams::TeamColor_XQZ);
			Option::Save(config["chams.local_color_xqz"], chams::LocalColor_XQZ);
			Option::Save(config["chams.dropped_weapons_color"], chams::ColorWeaponDroppedChams);
			Option::Save(config["chams.plantedc4_color"], chams::colorPlantedC4Chams);
			Option::Save(config["chams.nades_color"], chams::colorNadeChams);
			Option::Save(config["chams.visible_color"], chams::visible_color);
			Option::Save(config["chams.occluded_color"], chams::occluded_color);
			Option::Save(config["chams.arms.color"], chams::arms::color);
			Option::Save(config["chams.plantedc4_color"], settings::chams::ChamsPlantedC4XQZ);
			Option::Save(config["chams.he_color"], settings::chams::ChamsHEXQZ);
			Option::Save(config["chams.molotov_inc_color"], settings::chams::ChamsMolotovIncendiaryXQZ);
			Option::Save(config["chams.flashbang_color"], settings::chams::ChamsFlashbangXQZ);
			Option::Save(config["chams.decoy_color"], settings::chams::ChamsDecoyXQZ);
			Option::Save(config["chams.smoke_color"], settings::chams::ChamsSmokeXQZ);
			Option::Save(config["chams.C4dropped_color"], settings::chams::ChamsC4Dropped);

			config["misc.bhop"] = misc::bhop;
			config["misc.auto_strafe"] = misc::auto_strafe;
			config["misc.viewmodel_fov"] = misc::viewmodel_fov;
			config["misc.moon_walk"] = misc::moon_walk;
			config["misc.deathmatch"] = misc::deathmatch;
			config["misc.no_flash"] = misc::no_flash;
			config["misc.no_smoke"] = misc::no_smoke;
			config["misc.knife_bot"] = misc::knife_bot;
			config["misc.radar"] = misc::radar;
			config["misc.human_bhop"] = misc::human_bhop;
			config["misc.human_bhop_hopchance"] = misc::bhop_hit_chance;
			config["misc.human_bhop_hopslimit"] = misc::hops_restricted_limit;
			config["misc.human_bhop_maxhops_hit"] = misc::max_hops_hit;
			config["misc.resolver"] = desync::resolver;
			config["misc.engine_prediction"] = movement::engine_prediction;
			config["misc.knife_bot"] = misc::knife_bot;
			config["misc.debug_fov"] = misc::debug_fov;
			config["misc.skychanger"] = visuals::skychanger;
			config["misc.skychanger_mode"] = visuals::skychanger_mode;
			config["misc.menu_style"] = visuals::newstyle;
			config["misc.rcs_cross"] = visuals::rcs_cross;
			config["misc.rcs_cross_mode"] = visuals::rcs_cross_mode;
			config["misc.bullet_tracer"] = visuals::bullet_tracer;
			config["misc.radius"] = visuals::radius;
			config["misc.no_scope_overlay"] = misc::noscope;
			config["misc.self_nade"] = misc::selfnade;
			config["misc.hitmarker"] = visuals::hitmarker;
			config["misc.hitmarker_sound"] = visuals::hitsound;
			config["misc.bomb_damage_esp"] = esp::bomb_esp;
			config["misc_kevlar_info"] = settings::esp::kevlarinfo;
			config["misc_spread_crosshair"] = settings::visuals::spread_cross;
			config["misc_esp_money"] = settings::esp::money;
			config["misc_esp_ammo"] = settings::esp::ammo;
			config["misc_esp_kit"] = settings::esp::haskit;
			config["misc.choke_indicator"] = settings::visuals::choke;
			config["misc.player_info_box"] = settings::visuals::player_info_box;
			config["misc.grief_box"] = settings::visuals::grief_box;
			config["misc.disable_3rd_person_on_weapon"] = settings::misc::disable_on_weapon;
			config["misc.left_hand_knife"] = settings::misc::lefthandknife;
			config["misc.fast_stop"] = settings::misc::fast_stop;
			config["misc.viewmodel_mode"] = settings::visuals::viewmodel_mode;
			config["misc.viewmodel_override"] = settings::misc::override_viewmodel;
			config["misc.viewmodel_offset_x"] = settings::misc::viewmodel_offset_x;
			config["misc.viewmodel_offset_y"] = settings::misc::viewmodel_offset_y;
			config["misc.viewmodel_offset_z"] = settings::misc::viewmodel_offset_z;

			Option::Save(config["misc_spread_crosshair_color"], settings::visuals::spread_cross_color);
			Option::Save(config["mics.rcs_cross_color"], visuals::recoilcolor);
			Option::Save(config["misc.bullet_tracer_color"], settings::visuals::colorBulletTracer); //Here the bug happens

			config["glow.enemy"] = glow::glowEnemyEnabled;
			config["glow.team"] = glow::glowTeamEnabled;
			config["glow.dropped_weapons"] = glow::glowDroppedWeaponsEnabled;
			config["glow.plantedc4"] = glow::glowC4PlantedEnabled;
			config["glow.nades"] = glow::glowNadesEnabled;
			config["glow.override"] = glow::glowOverride;

			Option::Save(config["glow.enemy_color"], glow::glowEnemyColor);
			Option::Save(config["glow.team_color"], glow::glowTeamColor);
			Option::Save(config["glow.plantedc4_color"], glow::glowC4PlantedColor);
			Option::Save(config["glow.nades_color"], glow::glowNadesColor);
			Option::Save(config["glow.dropped_weapons_color"], glow::glowDroppedWeaponsColor);

			config["fake_lags.enabled"] = fake_lags::enabled;
			config["fake_lags.factor"] = fake_lags::factor;
			config["fake_lags.type"] = fake_lags::type;

			config["desync.enabled"] = desync::enabled;
			config["desync.yaw_flip"] = desync::yaw_flip;
			config["desync.resolver"] = desync::resolver;
			config["desync.enabled2"] = desync::enabled2;
			config["desync_mode"] = desync::desync_mode;
			config["desync.chams_mode"] = desync::desync_mode;

			config["legitbot.setting_type"] = aimbot::setting_type;

			Json::Value aimbot_items;
			for (auto data : aimbot::m_items)
			{
				Json::Value act;
				const auto aimbot_data = data.second;
				act["back_shot"] = aimbot_data.back_shot;
				act["autodelay"] = aimbot_data.autodelay;
				act["autopistol"] = aimbot_data.autopistol;
				act["autowall.enabled"] = aimbot_data.autowall.enabled;
				act["autowall.min_damage"] = aimbot_data.autowall.min_damage;
				act["backtrack.enabled"] = aimbot_data.backtrack.enabled;
				act["backtrack.legit"] = aimbot_data.backtrack.legit;
				act["backtrack.ticks"] = aimbot_data.backtrack.ticks;
				act["check_flash"] = aimbot_data.check_flash;
				act["check_smoke"] = aimbot_data.check_smoke;
				act["check_air"] = aimbot_data.check_air;
				act["enabled"] = aimbot_data.enabled;
				act["fov"] = aimbot_data.fov;
				act["dynamic_fov"] = aimbot_data.dynamic_fov;
				act["silent.enabled"] = aimbot_data.silent.enabled;
				act["silent.fov"] = aimbot_data.silent.fov;
				act["silent.always"] = aimbot_data.silent.always;
				act["silent.chanse"] = aimbot_data.silent.chanse;
				act["silent.with_smooth"] = aimbot_data.silent.with_smooth;
				act["by_damage"] = aimbot_data.by_damage;
				act["min_damage"] = aimbot_data.min_damage;
				act["hitchanse"] = aimbot_data.min_hitchanse;
				act["backtrack.time"] = aimbot_data.backtrack.time;
				act["rcs_hitbox_override"] = aimbot_data.rcs_override_hitbox;

				act["trigger.enabled"] = aimbot_data.trigger.enabled;
				act["trigger.in_radius"] = aimbot_data.trigger.in_radius;
				act["trigger.delay"] = aimbot_data.trigger.delay;
				act["trigger.delay_btw_shots"] = aimbot_data.trigger.delay_btw_shots;
				act["trigger.hitchance"] = aimbot_data.trigger.hitchance;

				act["hitboxes.head"] = aimbot_data.hitboxes.head;
				act["hitboxes.neck"] = aimbot_data.hitboxes.neck;
				act["hitboxes.body"] = aimbot_data.hitboxes.body;
				act["hitboxes.hands"] = aimbot_data.hitboxes.hands;
				act["hitboxes.legs"] = aimbot_data.hitboxes.legs;

				act["kill_delay"] = aimbot_data.kill_delay;
				act["check_zoom"] = aimbot_data.check_zoom;
				act["shot_delay"] = aimbot_data.shot_delay;
				act["smooth"] = aimbot_data.smooth;
				act["min_damage"] = aimbot_data.min_damage;

				act["rcs"] = aimbot_data.recoil.enabled;
				act["rcs.humanize"] = aimbot_data.recoil.humanize;
				act["rcs.first_bullet"] = aimbot_data.recoil.first_bullet;
				act["rcs.standalone"] = aimbot_data.recoil.standalone;
				act["rcs_value_x"] = aimbot_data.recoil.pitch;
				act["rcs_value_y"] = aimbot_data.recoil.yaw;

				aimbot_items[data.first] = act;
			}

			config["legitbot.items"] = aimbot_items;

			Json::Value aimbot_groups;
			for (auto group : aimbot::m_groups)
			{
				Json::Value act;
				act["name"] = group.name;

				Json::Value weapons;
				for (auto& weapon : group.weapons)
					weapons.append(weapon);

				act["weapons"] = weapons;

				aimbot_groups.append(act);
			}

			config["aimbot.groups"] = aimbot_groups;

			return config;
		});
	}
}