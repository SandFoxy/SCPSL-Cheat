#include <cstdint>
#include <cstdint>
#include <vector>
#include <string>
#include "../../Math/math.h"
#include "../../Engine/engine.h"
#include "../../../xorstr.hpp"


enum ItemType
{
	None = -1,
	KeycardJanitor,
	KeycardScientist,
	KeycardResearchCoordinator,
	KeycardZoneManager,
	KeycardGuard,
	KeycardMTFPrivate,
	KeycardContainmentEngineer,
	KeycardMTFOperative,
	KeycardMTFCaptain,
	KeycardFacilityManager,
	KeycardChaosInsurgency,
	KeycardO5,
	Radio,
	GunCOM15,
	Medkit,
	Flashlight,
	MicroHID,
	SCP500,
	SCP207,
	Ammo12gauge,
	GunE11SR,
	GunCrossvec,
	Ammo556x45,
	GunFSP9,
	GunLogicer,
	GrenadeHE,
	GrenadeFlash,
	Ammo44cal,
	Ammo762x39,
	Ammo9x19,
	GunCOM18,
	SCP018,
	SCP268,
	Adrenaline,
	Painkillers,
	Coin,
	ArmorLight,
	ArmorCombat,
	ArmorHeavy,
	GunRevolver,
	GunAK,
	GunShotgun,
	SCP330,
	SCP2176,
	SCP244a,
	SCP244b,
	SCP1853,
	ParticleDisruptor,
	GunCom45,
	SCP1576,
	Jailbird,
	AntiSCP207,
	GunFRMG0,
	GunA7,
	Lantern,
	Snowball,
	Coal,
	SpecialCoal,
	Tape
};

int CalculateDistanceToLocal12(Vector3 pos1) {
	return std::round(Internal::CallPointer<float>(offsets.Vector3_Distance, pos1, engine.Client.LocalPlayer.Position));// Vector3.Distance | DnSpy
}
bool isValidItem(ItemType itemType) {
	switch (itemType) {
		// Keycards
	case KeycardJanitor:
	case KeycardScientist:
	case KeycardResearchCoordinator:
	case KeycardZoneManager:
	case KeycardGuard:
	case KeycardMTFPrivate:
	case KeycardContainmentEngineer:
	case KeycardMTFOperative:
	case KeycardMTFCaptain:
	case KeycardFacilityManager:
	case KeycardChaosInsurgency:
	case KeycardO5:
		return cfg.showkeycards;

		// Radios
	case Radio:
		return false;

		// Guns
	case MicroHID:
	case ParticleDisruptor:
	case Jailbird:
	case GunCOM15:
	case GunE11SR:
	case GunCrossvec:
	case GunFSP9:
	case GunLogicer:
	case GunCOM18:
	case GunRevolver:
	case GunAK:
	case GunShotgun:
	case GunCom45:
	case GunFRMG0:
	case GunA7:
		return cfg.showweapon;

		// Ammo
	case GrenadeHE:
	case GrenadeFlash:
	case Ammo12gauge:
	case Ammo556x45:
	case Ammo44cal:
	case Ammo762x39:
	case Ammo9x19:
		return cfg.showammo;

		// Medkit and Healing
	case SCP500:
	case Medkit:
	case Adrenaline:
	case Painkillers:
		return cfg.showmeds;

	case ArmorLight:
	case ArmorCombat:
	case ArmorHeavy:
		return cfg.showarmor;

		// SCP Items
	case SCP207:
	case SCP018:
	case SCP268:
	case SCP330:
	case SCP2176:
	case SCP244a:
	case SCP244b:
	case SCP1853:
	case SCP1576:
	case AntiSCP207:
		return cfg.showscps;

		//Other Items
	case Lantern:
	case Snowball:
	case Coal:
	case SpecialCoal:
	case Tape:
		return cfg.showother;

	default:
		return false;
	}
};
namespace ItemPickupBase {
	//class ItemToSnipe {
	//public:
	//	Vector3 Position;
	//	std::string itemname = xorstr_("Item");
	//	ItemType itemType;
	//};
	class Item {
	public:
		Vector3 Position;
		ImColor ItemColor = ImColor(0, 255, 135);
		std::string itemname = xorstr_("Item");
		UnityEngine_GameObject_o* GameObject;
		ItemType itemType;
		void FindItem(ItemType itemType)
		{
			this->itemType = itemType;
			switch (itemType)
			{
			case KeycardJanitor:
				this->itemname = xorstr_("Janitor Keycard");
				break;
			case KeycardScientist:
				this->itemname = xorstr_("Sntist Keycard");
				break;
			case KeycardResearchCoordinator:
				this->itemname = xorstr_("Major Researcher Keycard");
				break;
			case KeycardZoneManager:
				this->itemname = xorstr_("Zone Manager Keycard");
				break;
			case KeycardGuard:
				this->itemname = xorstr_("Facility Guard Keycard");
				break;
			case KeycardMTFPrivate:
				this->itemname = xorstr_("Private Keycard");
				break;
			case KeycardContainmentEngineer:
				this->itemname = xorstr_("Containment Engineer Keycard");
				break;
			case KeycardMTFOperative:
				this->itemname = xorstr_("Sergeant Keycard");
				break;
			case KeycardMTFCaptain:
				this->itemname = xorstr_("Captain Keycard");
				break;
			case KeycardFacilityManager:
				this->itemname = xorstr_("Facility Manager Keycard");
				break;
			case KeycardChaosInsurgency:
				this->itemname = xorstr_("Chaos Insurgency Hacking Device");
				break;
			case KeycardO5:
				this->itemname = xorstr_("O5 Keycard");
				this->ItemColor = ImColor(0, 0, 0);
				break;
			case Radio:
				this->itemname = xorstr_("Radio");
				break;
			case GunCOM15:
				this->itemname = xorstr_("Scientist Pistol");
				this->ItemColor = ImColor(255, 0, 0);
				break;
			case Medkit:
				this->itemname = xorstr_("Medkit");
				this->ItemColor = ImColor(255, 21, 0);
				break;
			case Flashlight:
				this->itemname = xorstr_("Flashlight");
				break;
			case MicroHID:
				this->itemname = xorstr_("Micro-HID");
				this->ItemColor = ImColor(0, 170, 255);
				break;
			case SCP500:
				this->itemname = xorstr_("SCP-500");
				break;
			case SCP207:
				this->itemname = xorstr_("Coca Cola");
				this->ItemColor = ImColor(100, 55, 0);
				break;
			case AntiSCP207:
				this->itemname = xorstr_("Pepsi");
				this->ItemColor = ImColor(0, 170, 255);
				break;
			case Ammo12gauge:
				this->itemname = xorstr_("12 gauge");
				break;
			case GunE11SR:
				this->itemname = xorstr_("AR-15");
				break;
			case GunCrossvec:
				this->itemname = xorstr_("Crossvec");
				break;
			case Ammo556x45:
				this->itemname = xorstr_("556x45");
				break;
			case GunFSP9:
				this->itemname = xorstr_("FSP-9");
				break;
			case GunLogicer:
				this->itemname = xorstr_("Logicer");
				break;
			case GrenadeHE:
				this->itemname = xorstr_("HE Grenade");
				break;
			case GrenadeFlash:
				this->itemname = xorstr_("Flash Grenade");
				break;
			case Ammo44cal:
				this->itemname = xorstr_("44 cal");
				break;
			case Ammo762x39:
				this->itemname = xorstr_("762x39");
				break;
			case Ammo9x19:
				this->itemname = xorstr_("9x19");
				break;
			case GunCOM18:
				this->itemname = xorstr_("Scientist Pistol");
				this->ItemColor = ImColor(255, 0, 0);
				break;
			case SCP018:
				this->itemname = xorstr_("SCP-018");
				break;
			case SCP268:
				this->itemname = xorstr_("Invisible Cap");
				this->ItemColor = ImColor(255, 188, 0);
				break;
			case Adrenaline:
				this->itemname = xorstr_("Adrenaline");
				break;
			case Painkillers:
				this->itemname = xorstr_("Painkillers");
				break;
			case Coin:
				this->itemname = xorstr_("Coin");
				break;
			case Snowball:
				this->itemname = xorstr_("Snowball");
				break;
			case ArmorLight:
				this->itemname = xorstr_("Light Armor");
				break;
			case ArmorCombat:
				this->itemname = xorstr_("Combat Armor");
				break;
			case ArmorHeavy:
				this->itemname = xorstr_("Heavy Armor");
				this->ItemColor = ImColor(255, 0, 0);
				break;
			case GunRevolver:
				this->itemname = xorstr_("Revolver");
				break;
			case GunAK:
				this->itemname = xorstr_("AK-47");
				break;
			case GunShotgun:
				this->itemname = xorstr_("Shotgun");
				break;
			case GunFRMG0:
				this->itemname = xorstr_("Captains Machine Gun");
				this->ItemColor = ImColor(0, 170, 255);
				break;
			case SCP330:
				this->itemname = xorstr_("SCP-330");
				break;
			case SCP2176:
				this->itemname = xorstr_("SCP-2176");
				break;
			case SCP244a:
				this->itemname = xorstr_("SCP-244-A");
				break;
			case SCP244b:
				this->itemname = xorstr_("SCP-244-B");
				break;
			case SCP1853:
				this->itemname = xorstr_("SCP-1853");
				break;
			case SCP1576:
				this->itemname = xorstr_("SCP-1576");
				break;
			case Coal:
				this->itemname = xorstr_("Coal");
				break;
			case SpecialCoal:
				this->itemname = xorstr_("Special Coal");
				break;
			case Jailbird:
				this->itemname = xorstr_("Jailbird");
				this->ItemColor = ImColor(0, 170, 255);
				break;
			case ParticleDisruptor:
				this->itemname = xorstr_("Particle Disruptor");
				this->ItemColor = ImColor(0, 170, 255);
				break;
			case Tape:
				this->itemname = xorstr_("Flamingo Player");
				this->ItemColor = ImColor(200, 0, 255);
				break;
			default:
				this->itemname = xorstr_("Item");
				break;
			}
		}
		bool IsKeyCard() {
			switch (this->itemType)
			{
			case KeycardJanitor:
				return true;
				break;
			case KeycardScientist:
				return true;
				break;
			case KeycardResearchCoordinator:
				return true;
				break;
			case KeycardZoneManager:
				return true;
				break;
			case KeycardGuard:
				return true;
				break;
			case KeycardMTFPrivate:
				return true;
				break;
			case KeycardContainmentEngineer:
				return true;
				break;
			case KeycardMTFOperative:
				return true;
				break;
			case KeycardMTFCaptain:
				return true;
				break;
			case KeycardFacilityManager:
				return true;
				break;
			case KeycardChaosInsurgency:
				return true;
				break;
			case KeycardO5:
				return true;
				break;
			default:
				return false;
				break;
			}
		}
		bool BetterThan(ItemType betterthan) {//No Check For KeyCard!
			if ((int) betterthan < (int) this->itemType)
				return true;
			return false;
		}
	};
	Item HoldItem;
	Item ItemToSnipe;
	std::vector<Item> items;

	typedef void(__fastcall* PickupStandardPhysicsUpdate)(InventorySystem_Items_Pickups_PickupStandardPhysics_o);
	PickupStandardPhysicsUpdate oPickupStandardPhysicsUpdate = 0;

	

	void PickupStandardPhysics_hk(InventorySystem_Items_Pickups_PickupStandardPhysics_o component) {

		if (!cfg.itemesp || cfg.adminmode)
			return oPickupStandardPhysicsUpdate(component);
		
		Vector3 Position = Internal::CallPointer<Vector3>(offsets.get_LastWorldPos, component);

		int distance = CalculateDistanceToLocal12(Position);
		
		if (distance > 200)
			return oPickupStandardPhysicsUpdate(component);
		 
		//component.klass.
		Item ThisItem;
		InventorySystem_Items_Pickups_ItemPickupBase_o* ItemPickupBase = Internal::CallPointer<InventorySystem_Items_Pickups_ItemPickupBase_o*>(offsets.get_Pickup, component);//InventorySystem.Items.Pickups.PickupStandardPhysics$$get_Pickup
		


		if (!isValidItem((ItemType)ItemPickupBase->fields.Info.fields.ItemId))
			return oPickupStandardPhysicsUpdate(component);

		//UnityEngine_Component_o* GeneratorComponent = reinterpret_cast<UnityEngine_Component_o*>(ItemPickupBase);

		InventorySystem_Items_Pickups_PickupSyncInfo_o* ItemSyncInfo = Internal::CallPointer<InventorySystem_Items_Pickups_PickupSyncInfo_o*>(offsets.PickupSyncInfo_NetworkInfo, component);
		
		ThisItem.Position = Position;

		ThisItem.FindItem((ItemType) ItemPickupBase->fields.Info.fields.ItemId);

		if (distance <= 2 && ThisItem.IsKeyCard() && ThisItem.BetterThan(HoldItem.itemType)) {
			if (ThisItem.BetterThan(ItemToSnipe.itemType)) {
				ItemToSnipe = ThisItem;
				ThisItem.ItemColor = ImColor(255, 0, 0);
				ThisItem.itemname.append(xorstr_(" Snipeing..."));
			}
		}
		else {
			ThisItem.itemname.append(std::string(xorstr_(" - ") + std::to_string(distance) + xorstr_("m")));
		}

		items.push_back(ThisItem);
		oPickupStandardPhysicsUpdate(component);
	}
}
namespace PocketTeleport {
	class Teleport {
	public:
		Vector3 Position;
		std::string firsttext = xorstr_("Killer");
	};
	std::vector<Teleport> teleports;
}
namespace Generator {
	class GeneratorObject {
	public:
		Vector3 Position;
		std::string firsttext = xorstr_("Generator");
		std::string secoundtext = xorstr_("NULL");
		bool Activating = false;
		bool Engaged = false;
	};

	typedef void(__fastcall* GeneratorUpdate)(MapGeneration_Distributors_Scp079Generator_o*);
	GeneratorUpdate Original = 0;

	std::vector<GeneratorObject> Generators;

	void Update(MapGeneration_Distributors_Scp079Generator_o* component) {
		if (!cfg.generatoresp)
			return Original(component);

		Generator::GeneratorObject GeneratorObj;
		UnityEngine_Transform_o* Transform;

		UnityEngine_Component_o* GeneratorComponent = reinterpret_cast<UnityEngine_Component_o*>(component);

		UnityEngine_Transform_o* GeneratorTransform = UnityEngine_Transform_o::GetTransform(GeneratorComponent);
		Vector3 Cached = GeneratorTransform->get_position();
		GeneratorObj.Position = Vector3(Cached.x, Cached.y + 1.5, Cached.z);

		int distance = CalculateDistanceToLocal12(GeneratorObj.Position);
		if (distance > 500 || !engine.Client.LocalPlayer.Alive)
			return Original(component);

		if (!Internal::CallPointer<bool>(offsets.GeneratorEngaged, component)) {
			if (Internal::CallPointer<bool>(offsets.GeneratorActivating, component)) {
				int TimerRemaining = Internal::CallPointer<int>(offsets.GeneratorTimerRemaining, component);

				GeneratorObj.Activating = true;
				GeneratorObj.secoundtext = std::to_string(TimerRemaining) + xorstr_("s");
				//LOG(std::to_string(TimerRemaining));
			}
			else {
				GeneratorObj.Activating = false;
				GeneratorObj.secoundtext = std::to_string(distance);
			}
		}
		else {
			GeneratorObj.Activating = false;
			GeneratorObj.Engaged = true;
			GeneratorObj.secoundtext = xorstr_("Engaged");
		}

		Generators.push_back(GeneratorObj);

		Original(component);
	}
}