// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Singleton containing native Gameplay Tags
 */
struct FGameplayTags
{
public:

   static const FGameplayTags& Get(){return GameplayTags;}

 //Function to initialize tags
    static void InitializeNativeGameplayTags();
	
	//Enemy Tag
	FGameplayTag PremiumEnemy;
	
	//Weapon Tags
	FGameplayTag Weapon_Melee;
	FGameplayTag Weapon_Melee_Dash;
	FGameplayTag Weapon_Range_Grill;
	FGameplayTag Weapon_Range_Boil;
	FGameplayTag Weapon_Range_Fry;
	FGameplayTag Weapon_Ingredient_Cheese;
	FGameplayTag Weapon_Ingredient_Wine;
	FGameplayTag Weapon_Ingredient_Jam;
	FGameplayTag Weapon_CanObtainBoil;
	FGameplayTag Weapon_CanObtainFry;

	//States Tags
	FGameplayTag States_AimBlocked;
	FGameplayTag States_Alert;
	FGameplayTag States_AllowStun;
	FGameplayTag States_BloodExplosion;
	FGameplayTag States_Block_Spores;
	FGameplayTag States_Burnt;
	FGameplayTag States_Channeling;
	FGameplayTag States_Charge;
	FGameplayTag States_Chase;
	FGameplayTag States_Cooked;
	FGameplayTag States_Cook_Grilled;
	FGameplayTag States_Cook_Boiled;
	FGameplayTag States_Cook_Fried;
	FGameplayTag States_Cooldown;
	FGameplayTag States_Cuts_Skewered;
	FGameplayTag States_Cuts_Sliced;
	FGameplayTag States_Cuts_Smashed;
	FGameplayTag States_Damaged;
	FGameplayTag States_Dead;
	FGameplayTag States_DesperatelyFlee;
	FGameplayTag States_Drunk;
	FGameplayTag States_ErraticDodge;
	FGameplayTag States_Escaped;
	FGameplayTag States_Flee;
	FGameplayTag States_HitReact;
	FGameplayTag States_IA_CannotUseTail;
	FGameplayTag States_IA_FalseFlee;
	FGameplayTag States_IA_Investigating;
	FGameplayTag States_IA_UsedFalseFlee;
	FGameplayTag States_IA_BlockTailFalseFlee;
	FGameplayTag States_IdleWander;
	FGameplayTag States_Ingredient_Cheese;
	FGameplayTag States_Ingredient_Wine;
	FGameplayTag States_Ingredient_Jam;
	FGameplayTag States_InProcessOfPetrification;
	FGameplayTag States_InPetrification;
	FGameplayTag States_Invulnerable;
	FGameplayTag States_Jump;
	FGameplayTag States_Laugh;
	FGameplayTag States_LowHealth;
	FGameplayTag States_Cackle;
	FGameplayTag States_PlayerFiring;
	FGameplayTag States_PlayerSingleFire;
	FGameplayTag States_PlayerShield;
	FGameplayTag States_PlayerRecolection;
	FGameplayTag States_Poisoned;
	FGameplayTag States_Regenerating;
	FGameplayTag States_ResetEnemy;
	FGameplayTag States_Raw;
	FGameplayTag States_Stun;
	FGameplayTag States_Soaked;
	FGameplayTag States_Vulnerable;
	FGameplayTag States_Slowdown;
	FGameplayTag States_WeaponsBlocked;
	FGameplayTag States_OnDashAttack;

	//Resource Tags
	FGameplayTag ResourceType_Fry;
	FGameplayTag ResourceType_Boil;
	FGameplayTag ResourceType_Grill;
	
	//Events Tags
	FGameplayTag Events_Common_AttackAbility;
	FGameplayTag Events_Common_StunAbility;
	FGameplayTag Events_Enemy_Cackle;
	FGameplayTag Events_Enemy_CallFriend;
	FGameplayTag Events_Enemy_Channeling;
	FGameplayTag Events_Enemy_Charge;
	FGameplayTag Events_Enemy_ChargeYeti;
	FGameplayTag Events_Enemy_Chase;
	FGameplayTag Events_Enemy_CheckDie;
	FGameplayTag Events_Enemy_CucaferaBite;
	FGameplayTag Events_Enemy_DesesperatelyFlee;
	FGameplayTag Events_Enemy_ErraticDodge;
	FGameplayTag Events_Enemy_Flee;
	FGameplayTag Events_Enemy_GuardianWall;
	FGameplayTag Events_Enemy_IdleWander;
	FGameplayTag Events_Enemy_InQTE;
	FGameplayTag Events_Enemy_KageBushinNoJutsu;
	FGameplayTag Events_Enemy_Kamikaze;
	FGameplayTag Events_Enemy_Peck;
	FGameplayTag Events_Enemy_PoisonSpit;
	FGameplayTag Events_Enemy_RangeShootAttack;
	FGameplayTag SeismicRotation;
	FGameplayTag Events_Enemy_SmokeAbility;
	FGameplayTag Events_Enemy_SpawnSpores;
	FGameplayTag Events_Enemy_SuicideJump;
	FGameplayTag Events_Enemy_SwordAttack;
	FGameplayTag Events_Enemy_TailSweep;
	FGameplayTag Events_Enemy_ToggleFlight;
	FGameplayTag Events_Enemy_SeismicWaveOriented;
	FGameplayTag Events_Enemy_SeismicRotation;
	FGameplayTag Events_Enviroment_SpawnSpores;
	FGameplayTag Events_Player_Tutorial;
	FGameplayTag Events_Player_Interact;
	FGameplayTag Events_Player_UseAmmo;
	FGameplayTag Events_Player_Finisher;
	FGameplayTag Events_Player_QTEFail;
	FGameplayTag Events_Player_HitReact;
	FGameplayTag Events_Weapon_JamSlam;
	FGameplayTag Events_Passive_PetrificationPassive;
	FGameplayTag Events_Player_Ranged;
	FGameplayTag Events_Weapon_RangedAiming;
	FGameplayTag Events_Weapon_Melee;
	FGameplayTag Events_Weapon_Boil;
	FGameplayTag Events_Weapon_Fry;
	FGameplayTag Events_Weapon_Grill;
	FGameplayTag Events_Weapon_CheeseMine;
	FGameplayTag Events_Weapon_WineSplash;
	FGameplayTag Events_Weapon_FinishMeleeWeaponAbility;
	FGameplayTag Events_Weapon_FinishRangeWeaponAbility;
	FGameplayTag Events_Weapon_FinishIngredientWeaponAbility;
	
	//Cue Tags
	FGameplayTag GameplayCue_Player_Arrow;
	FGameplayTag GameplayCue_Basilico_MineCue;
	FGameplayTag GameplayCue_Combat_CleaverLoop;
	FGameplayTag GameplayCue_Combat_EnemyAshesDeath;
	FGameplayTag GameplayCue_Combat_GhunterGrunt;
	FGameplayTag GameplayCue_Combat_CleaverAttack;
	FGameplayTag GameplayCue_Combat_GrillSound;
	FGameplayTag GameplayCue_Combat_GrillVFX;
	FGameplayTag GameplayCue_Combat_GrillTickSFX;
	FGameplayTag GameplayCue_Combat_BoilSound;
	FGameplayTag GameplayCue_Combat_BoilVFX;
	FGameplayTag GameplayCue_Combat_FryLoop;
	FGameplayTag GameplayCue_Combat_FryShoot;
	FGameplayTag GameplayCue_Combat_EnemyBurnt;
	FGameplayTag GameplayCue_Combat_EnemyHit;
	FGameplayTag GameplayCue_Combat_EnemyDeath;
	FGameplayTag GameplayCue_Combat_FryerExplosion;
	FGameplayTag GameplayCue_Combat_GrillFireLoop;
	FGameplayTag GameplayCue_Ingredient_Change;
	FGameplayTag GameplayCue_Ingredient_CheeseExplosion;
	FGameplayTag GameplayCue_Ingredient_CheesePlant;
	FGameplayTag GameplayCue_Ingredient_CheeseSuccesfulPlant;
	FGameplayTag GameplayCue_Ingredient_WineSplashExplosion;
	FGameplayTag GameplayCue_Ingredient_WineDrunkVulnerableEffect;
	FGameplayTag GameplayCue_Ingredient_JamExplosion;
	FGameplayTag GameplayCue_Ingredient_JamPetrify;
	FGameplayTag GameplayCue_Ingredient_InProcessOfJamPetrify;
	FGameplayTag GameplayCue_QTE_QTEStart;
	FGameplayTag GameplayCue_QTE_QTESelection;
	FGameplayTag GameplayCue_QTE_QTEFailedInput;
	FGameplayTag GameplayCue_QTE_HitSound1;
	FGameplayTag GameplayCue_QTE_HitSound2;
	FGameplayTag GameplayCue_QTE_OnHitBloodVFX;
	FGameplayTag GameplayCue_QTE_FinisherBloodVFX;
	FGameplayTag GameplayCue_PlayerShieldBroken;
	FGameplayTag GameplayCue_Recipe_Completed;
	FGameplayTag GameplayCue_Weapon_Change;
	FGameplayTag GameplayCue_Range_Change;

	FGameplayTag GameplayCue_Enemy_GrillVFX;
	FGameplayTag GameplayCue_Enemy_BoilVFX;

	//Enemy Names Tags
	FGameplayTag Enemy_Gamusino;
	FGameplayTag Enemy_Gamusino_Tutorial;
	FGameplayTag Enemy_Gamusino_Kami;
	FGameplayTag Enemy_Gamusino_Muddy;
	FGameplayTag Enemy_Gamusino_Premium;
	FGameplayTag Enemy_Gamusino_Yeti;

	FGameplayTag Enemy_Cucafera;
	FGameplayTag Enemy_Cucafera_Spectral;
	FGameplayTag Enemy_Cucafera_Guardian;
	FGameplayTag Enemy_Cucafera_Jumper;

	FGameplayTag Enemy_Cuelebre;

	FGameplayTag Enemy_Basilisco;
	FGameplayTag Enemy_Basilisco_Brute;
	FGameplayTag Enemy_Basilisco_Premium;
	FGameplayTag Enemy_Basilisco_Toxic;

	//Identify Effects

	FGameplayTag Modify_Speed;

	//Type Tags
	FGameplayTag Type_State;
	FGameplayTag Type_Attack;

	//NPC Tags
	FGameplayTag NPC_Gailor;
	FGameplayTag NPC_Gavern;
	FGameplayTag NPC_Gmith;
	FGameplayTag NPC_Goctor;
	FGameplayTag NPC_Gooker;
	FGameplayTag NPC_Guncle;

	//Notification Tags
	FGameplayTag Notification_FullInventory;
	FGameplayTag Notification_OutOfGrillAmmo;
	FGameplayTag Notification_OutOfBoilAmmo;
	FGameplayTag Notification_OutOfFryAmmo;
	FGameplayTag Notification_OutOfCheeseAmmo;
	FGameplayTag Notification_OutOfJamAmmo;
	FGameplayTag Notification_OutOfWineAmmo;
	FGameplayTag Notification_Poison;
	FGameplayTag Notification_Invisible;
	FGameplayTag Notification_NeedAnotherMelee;

	
protected:

 
public:
	static FGameplayTags GameplayTags;
};
