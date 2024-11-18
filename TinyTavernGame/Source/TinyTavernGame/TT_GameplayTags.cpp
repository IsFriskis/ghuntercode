// Fill out your copyright notice in the Description page of Project Settings.


#include "TT_GameplayTags.h"
#include "GameplayTagsManager.h"

FGameplayTags FGameplayTags::GameplayTags;


void FGameplayTags::InitializeNativeGameplayTags()
{
	//----------------------------------------------------------------------------------------------------------------------------------
	//-------------------------------------Enemy Premium Tags----------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------------------------------------
	GameplayTags.PremiumEnemy = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("PremiumEnemy"),
		FString("The enemy is premium"));
	
	//----------------------------------------------------------------------------------------------------------------------------------
	//-------------------------------------Weapon Tags----------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------------------------------------
	
	GameplayTags.Weapon_Melee = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Weapon.Melee"),
		FString("Weapon Melee"));

	GameplayTags.Weapon_Melee_Dash = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Weapon.Melee.Dash"),
		FString("Weapon Melee Dash"));
	
	GameplayTags.Weapon_Range_Boil = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Weapon.Range.Boil"),
	FString("Weapon Range Boil"));

	GameplayTags.Weapon_Range_Fry = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Weapon.Range.Fry"),
		FString("Weapon Range Fry"));

	GameplayTags.Weapon_Range_Grill = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Weapon.Range.Grill"),
		FString("Weapon Range Grill"));

	GameplayTags.Weapon_Ingredient_Cheese = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Weapon.Ingredient.Cheese"),
	FString("Weapon Ingredient Cheese"));

	GameplayTags.Weapon_Ingredient_Wine = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Weapon.Ingredient.Wine"),
		FString("Weapon Ingredient Wine"));
	
	GameplayTags.Weapon_Ingredient_Jam = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Weapon.Ingredient.Jam"),
	FString("Weapon Ingredient Jam"));

	GameplayTags.Weapon_CanObtainBoil = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Weapon.CanObtainBoil"),
	FString("Weapon Can Obtain Boil"));

	GameplayTags.Weapon_CanObtainFry = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Weapon.CanObtainFry"),
	FString("Weapon Can Obtain Fry"));


	//----------------------------------------------------------------------------------------------------------------------------------
	//---------------------------------Resource Type Tags-------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------------------------------------
	GameplayTags.ResourceType_Boil = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("ResourceType.Boil"),
		FString("Resource Type Boil"));

	GameplayTags.ResourceType_Fry = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("ResourceType.Fry"),
		FString("Resource Type Fry"));

	GameplayTags.ResourceType_Grill = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("ResourceType.Grill"),
		FString("Resource Type Grill"));

	
	
	//----------------------------------------------------------------------------------------------------------------------------------
	//-------------------------------------States Tags----------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------------------------------------
	
	GameplayTags.States_AimBlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.AimBlocked"),
	FString("Aim blocker"));

	GameplayTags.States_AllowStun = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.AllowStun"),
	FString("Tag required to apply StunEffect"));

	GameplayTags.States_Alert = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Alert"),
	FString("Tag granted when enemy is on Alert"));

	GameplayTags.States_BloodExplosion = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.BloodExplosion"),
	FString("Tag granted when enemy Die"));

	GameplayTags.States_Block_Spores = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Block.Spores"),
	FString("Tag granted when block the spore ability"));
	
	GameplayTags.States_Burnt = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Burnt"),
	FString("Tag granted when enemy is burned"));

	
	GameplayTags.States_Chase = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Chase"),
	FString("Tag granted when enemy is on Chase"));

	GameplayTags.States_Charge = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Charge"),
	FString("Tag granted when enemy is on Charge"));

	GameplayTags.States_Cooked = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Cooked"),
	FString("Tag granted when enemy is Cooked"));
	
	GameplayTags.States_Cook_Grilled = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Cook.Grilled"),
	FString("Grilled Cook State"));

	GameplayTags.States_Cook_Boiled = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Cook.Boiled"),
	FString("Boiled Cook State"));

	GameplayTags.States_Cook_Fried = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Cook.Fried"),
	FString("Fried Cook State"));

	GameplayTags.States_Cooldown = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Cooldown"),
	FString("Tag granted when cooldown"));
	
	GameplayTags.States_Cuts_Skewered = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Cuts.Skewered"),
	FString("Skewered Type Of Cut"));	
	
	GameplayTags.States_Cuts_Sliced = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Cuts.Sliced"),
	FString("Sliced Type Of Cut"));	
	
	GameplayTags.States_Cuts_Smashed = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Cuts.Smashed"),
	FString("Smashed Type Of Cut"));

	GameplayTags.States_Damaged = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Damaged"),
	FString("Damaged State"));

	GameplayTags.States_Dead = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Dead"),
	FString("Dead State"));

	GameplayTags.States_DesperatelyFlee = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.DesperatelyFlee"),
	FString("DesperatelyFlee State"));
	
	GameplayTags.States_Drunk = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Drunk"),
	FString("Drunk State"));

	GameplayTags.States_ErraticDodge = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.ErraticDodge"),
	FString("ErraticDodge State"));	

	GameplayTags.States_Escaped = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Escaped"),
	FString("Escaped State"));

	GameplayTags.States_Flee = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Flee"),
	FString("Tag granted when enemy is on Flee"));
	
	GameplayTags.States_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.HitReact"),
	FString("Tag granted when Hit Reacting"));

	GameplayTags.States_IA_CannotUseTail = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.IA.CannotUseTail"),
	FString("If Basilisco cannot use Tail Ability"));

	GameplayTags.States_IA_FalseFlee = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.IA.FalseFlee"),
	FString("State False Flee"));

	GameplayTags.States_IA_Investigating = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.IA.Investigating"),
	FString("State Investigating"));

	GameplayTags.States_IA_UsedFalseFlee = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.IA.UsedFalseFlee"),
	FString("If the enemy Used False Flee"));

	GameplayTags.States_IA_BlockTailFalseFlee = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.IA.BlockTailFalseFlee"),
	FString("If the enemy BlockTailFalseFlee"));

	GameplayTags.States_IdleWander = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.IdleWander"),
	FString("Tag granted when enemy is on Idle/Wander"));
	
	GameplayTags.States_Ingredient_Cheese = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Ingredients.Cheese"),
	FString("Cheese Ingredient State"));

	GameplayTags.States_Ingredient_Wine = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Ingredients.Wine"),
	FString("Wine Ingredient State"));

	GameplayTags.States_Ingredient_Jam = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Ingredients.Jam"),
	FString("Jam Ingredient State"));

	GameplayTags.States_InProcessOfPetrification = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.InProcessOfPetrification"),
	FString("In Process Of Petrification State"));

	GameplayTags.States_InPetrification = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.InPetrification"),
	FString("In Petrification State"));

	GameplayTags.States_Jump = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Jump"),
	FString("Jump State"));

	GameplayTags.States_Laugh = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Laugh"),
	FString("Tag granted when enemy is on Laugh"));

	GameplayTags.States_LowHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.LowHealth"),
	FString("Tag granted when Player have low health"));
	
	GameplayTags.States_Cackle = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Cackle"),
	FString("Tag granted when enemy is on Cackle"));
	
	GameplayTags.States_Poisoned = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Poisoned"),
	FString("Tag granted when is Poisoned"));
	
	GameplayTags.States_Regenerating = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Regenerating"),
	FString("Regenerating State"));

	GameplayTags.States_ResetEnemy = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.ResetEnemy"),
	FString("ResetEnemy State"));

	GameplayTags.States_Raw = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Raw"),
	FString("Raw State"));		

	GameplayTags.States_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Stun"),
	FString("Stun State"));

	GameplayTags.States_Soaked = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Soaked"),
	FString("Soaked State"));

	GameplayTags.States_Vulnerable = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Vulnerable"),
	FString("Vulnerable State"));

	GameplayTags.States_Vulnerable = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Invulnerable"),
	FString("Invulnerable State"));

	GameplayTags.States_Channeling = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Channeling"),
	FString("Channeling State"));
	
	GameplayTags.States_WeaponsBlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.WeaponsBlocked"),
	FString("Weapons change blocker"));

	GameplayTags.States_OnDashAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.OnDashAttack"),
	FString("OnDashAttack State"));

	GameplayTags.States_Slowdown = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.Slowdown"),
	FString("State slowdown"));

	GameplayTags.States_PlayerFiring = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.PlayerFiring"),
	FString("Player Firing"));

	GameplayTags.States_PlayerSingleFire = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.PlayerSingleFire"),
	FString("Player Single Fire"));

	GameplayTags.States_PlayerRecolection = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.PlayerRecolection"),
	FString("Player Recolection"));
	
	GameplayTags.States_PlayerShield = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("States.PlayerShield"),
	FString("Player Shield"));
	//----------------------------------------------------------------------------------------------------------------------------------
	//-------------------------------------Events Tags----------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------------------------------------
	
	GameplayTags.Events_Common_AttackAbility = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Common.AttackAbility"),
	FString("Event Attack Ability"));
	
	GameplayTags.Events_Common_StunAbility = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Common.StunAbility"),
	FString("Event Stun Ability"));	

	GameplayTags.Events_Enemy_Cackle = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.Cackle"),
	FString("Event Cackle Ability"));

	GameplayTags.Events_Enemy_CallFriend = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.CallFriend"),
	FString("Event Cucafera CallFriends Ability"));

	GameplayTags.Events_Enemy_ChargeYeti = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.ChargeYeti"),
	FString("Event Charge Yeti Enemy Ability"));

	GameplayTags.Events_Enemy_Chase = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.Chase"),
	FString("Event Chase Ability"));
	
	GameplayTags.Events_Enemy_CheckDie = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.CheckDie"),
	FString("Event CheckDie Enemy Ability"));

	GameplayTags.Events_Enemy_CucaferaBite = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.CucaferaBite"),
	FString("Event Cucafera Bite Ability"));
	
	GameplayTags.Events_Enemy_Flee = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.Flee"),
	FString("Event Flee Enemy Ability"));

	GameplayTags.Events_Enemy_GuardianWall = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.GuardianWall"),
	FString("Event GuardianWall Enemy Ability"));
	
	GameplayTags.Events_Enemy_Flee = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.DesesperatelyFlee"),
	FString("Event Desesperately Flee Enemy Ability"));

	GameplayTags.Events_Enemy_ErraticDodge = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.ErraticDodge"),
	FString("Event ErraticDodge Enemy Ability"));

	GameplayTags.Events_Enemy_IdleWander = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.IdleWander"),
	FString("Event Idle/Wander Enemy Ability"));

	GameplayTags.Events_Enemy_InQTE = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.InQTE"),
	FString("Event InQTE Enemy Ability"));
	
	GameplayTags.Events_Enemy_Flee = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.Channeling"),
	FString("Event Channeling Enemy Ability"));

	GameplayTags.Events_Enemy_PoisonSpit = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.PoisonSpit"),
	FString("Event PoisonSpit Enemy Ability"));

	GameplayTags.Events_Enemy_TailSweep = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.TailSweep"),
	FString("Event TailSweep Enemy Ability"));

	GameplayTags.Events_Enemy_Peck = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.Peck"),
	FString("Event Peck Enemy Ability"));

	GameplayTags.Events_Enemy_Kamikaze = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.Kamikaze"),
	FString("Event Kamikaze Enemy Ability"));

	GameplayTags.Events_Enemy_SwordAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.SwordAttack"),
	FString("Event SwordAttack Enemy Ability"));
	
	GameplayTags.Events_Enemy_RangeShootAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.RangeShootAttack"),
	FString("Event RangeShootAttack Enemy Ability"));

	GameplayTags.Events_Enemy_SeismicRotation = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.SeismicRotation"),
	FString("Event SeismicRotation Ability"));
	
	GameplayTags.Events_Enemy_SmokeAbility = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.SmokeAbility"),
	FString("Event SmokeAbility Enemy Ability"));
	
	GameplayTags.Events_Enemy_SpawnSpores = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.SpawnSpores"),
	FString("Event Spawn Spores Enemy Ability"));

	GameplayTags.Events_Enemy_SuicideJump = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.SuicideJump"),
	FString("Event Suicide Jump Enemy Ability"));

	GameplayTags.Events_Enemy_KageBushinNoJutsu = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.KageBushinNoJutsu"),
	FString("Event KageBushinNoJutsu Enemy Ability"));

	GameplayTags.Events_Enemy_ToggleFlight = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.ToggleFlight"),
	FString("Event Toggle Flight Enemy Ability"));

	GameplayTags.Events_Enemy_SeismicWaveOriented = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enemy.SeismicWaveOriented"),
	FString("Event SeismicWaveOriented Enemy Ability"));
	
	GameplayTags.Events_Enviroment_SpawnSpores = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Enviroment.SpawnSpores"),
	FString("Event Enviroment SpawnSpores Ability"));

	GameplayTags.Events_Player_Tutorial = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Player.Tutorial"),
	FString("Event Player In Tutorial"));
	
	GameplayTags.Events_Player_Interact = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Player.Interact"),
	FString("Event Player Interact"));

	GameplayTags.Events_Player_UseAmmo = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Player.UseAmmo"),
	FString("Subtract Ammo"));

	GameplayTags.Events_Player_Finisher = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Player.Finisher"),
	FString("Event Player Finisher"));

	GameplayTags.Events_Player_QTEFail = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Player.QTEFail"),
	FString("Event Player QTEFail"));

	GameplayTags.Events_Player_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Player.HitReact"),
	FString("Event Player HitReact"));

	GameplayTags.Events_Weapon_CheeseMine = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Weapon.CheeseMine"),
	FString("CheeseMine Attack"));
	
	GameplayTags.Events_Weapon_Melee = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Weapon.Melee"),
	FString("Melee Attack"));
	
	GameplayTags.Events_Weapon_Fry = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Weapon.Fry"),
	FString("Fry Attack"));

	GameplayTags.Events_Weapon_Boil = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Weapon.Boil"),
	FString("Boil Attack"));

	GameplayTags.Events_Weapon_Grill = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Weapon.Grill"),
	FString("Grill Attack"));
	
	GameplayTags.Events_Weapon_WineSplash = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Weapon.WineSplash"),
	FString("WineSplash Attack"));
	
	GameplayTags.Events_Weapon_JamSlam = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Weapon.JamSlam"),
	FString("JamSlam Attack"));

	GameplayTags.Events_Passive_PetrificationPassive = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Passive.PetrificationPassive"),
	FString("Petrification Passive "));
	
	GameplayTags.Events_Player_Ranged = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Player.Ranged"),
	FString("Ranged Attack"));

	GameplayTags.Events_Weapon_RangedAiming = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Weapon.RangedAiming"),
	FString("Ranged Aiming"));
	
	GameplayTags.Events_Weapon_FinishMeleeWeaponAbility = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Weapon.FinishWeaponAbility"),
	FString("Finish Weapon Ability"));

	GameplayTags.Events_Weapon_FinishRangeWeaponAbility = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Weapon.FinishRangeWeaponAbility"),
	FString("Finish Range Ability"));

	GameplayTags.Events_Weapon_FinishIngredientWeaponAbility = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Events.Weapon.FinishIngredientWeaponAbility"),
	FString("Finish Ingredient Ability"));

	//----------------------------------------------------------------------------------------------------------------------------------
	//-------------------------------------NPC Tags-------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------------------------------------
	
	GameplayTags.NPC_Goctor = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("NPC.Goctor"),
	FString("NPC Goctor"));

	GameplayTags.NPC_Gmith = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("NPC.Gmith"),
	FString("NPC Gmith"));

	GameplayTags.NPC_Gailor = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("NPC.Gailor"),
	FString("NPC Gailor"));

	GameplayTags.NPC_Gavern = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("NPC.Gavern"),
	FString("NPC Gavern"));

	GameplayTags.NPC_Gooker = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("NPC.Gooker"),
	FString("NPC Gooker"));

	GameplayTags.NPC_Guncle = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("NPC.Guncle"),
	FString("NPC Guncle"));

	//----------------------------------------------------------------------------------------------------------------------------------
	//-------------------------------------Cues Tags------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------------------------------------
	
	GameplayTags.GameplayCue_Player_Arrow = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Player.Arrow"),
		FString("Player Aim Arrow Cue"));

	GameplayTags.GameplayCue_Basilico_MineCue = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Basilico.MineCue"),
		FString("Player Basilico MineCue Cue"));
	
	GameplayTags.GameplayCue_Combat_CleaverLoop = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Combat.CleaverLoop"),
		FString("Cleaver Loop Cue"));
	GameplayTags.GameplayCue_Combat_EnemyAshesDeath = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Combat.EnemyAshesDeath"),
		FString("Enemy convert ashes Cue"));

	GameplayTags.GameplayCue_Combat_FryerExplosion = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("GameplayCue.Combat.FryerExplosion"),
			FString("FryerExplosion Cue"));
	
	GameplayTags.GameplayCue_Combat_GhunterGrunt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Combat.GhunterGrunt"),
		FString("GhunterGrunt Cue"));

	GameplayTags.GameplayCue_Combat_CleaverAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Combat.CleaverAttack"),
		FString("Cleaver Attack Cue"));

	GameplayTags.GameplayCue_Combat_GrillSound = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Combat.GrillSound"),
		FString("Grill Sound Cue"));

	GameplayTags.GameplayCue_Combat_GrillVFX = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Combat.GrillVFX"),
		FString("Grill VFX Cue"));

	GameplayTags.GameplayCue_Combat_GrillTickSFX = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Combat.GrillTickSFX"),
		FString("Grill Tick SFX Cue"));

	GameplayTags.GameplayCue_Combat_GrillFireLoop = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Combat.GrillFireLoop"),
		FString("GrillFireLoop Cue"));

	GameplayTags.GameplayCue_Combat_BoilSound = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.Combat.BoilSound"),
	FString("Boiler Sound Cue"));

	GameplayTags.GameplayCue_Combat_BoilVFX = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Combat.BoilVFX"),
		FString("Boiler VFX Cue"));

	GameplayTags.GameplayCue_Combat_FryLoop = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Combat.FryLoop"),
		FString("Fry Loop Cue"));

	GameplayTags.GameplayCue_Combat_FryShoot = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.Combat.FryShoot"),
		FString("Fry Shoot Cue"));
	
	GameplayTags.GameplayCue_Combat_EnemyHit = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.Combat.EnemyHit"),
	FString("EnemyHit Cue"));	
	
	GameplayTags.GameplayCue_Combat_EnemyDeath = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.Combat.EnemyDeath"),
	FString("EnemyDeath Cue"));	
	
	GameplayTags.GameplayCue_Combat_EnemyBurnt = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.Combat.EnemyBurnt"),
	FString("EnemyBurnt Cue"));

	GameplayTags.GameplayCue_Ingredient_CheeseExplosion = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.Ingredient.CheeseExplosion"),
	FString("Ingredient Cheese Explosion Cue"));

	GameplayTags.GameplayCue_Ingredient_CheesePlant = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.Ingredient.CheesePlant"),
	FString("Ingredient Cheese Plant Cue"));

	GameplayTags.GameplayCue_Ingredient_CheeseSuccesfulPlant = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.Ingredient.CheeseSuccesfulPlant"),
	FString("Ingredient Cheese Succesful Plant Cue"));
	
	GameplayTags.GameplayCue_Ingredient_WineSplashExplosion = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.Ingredient.WineSplashExplosion"),
	FString("Ingredient Wine Splash Explosion Cue"));

	GameplayTags.GameplayCue_Ingredient_WineDrunkVulnerableEffect = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.Ingredient.WineDrunkVulnerableEffect"),
	FString("Ingredient Wine Drunk VulnerableEffect Cue"));

	GameplayTags.GameplayCue_Ingredient_JamExplosion = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.Ingredient.JamExplosionEffect"),
	FString("Ingredient Jam Explosion Effect Cue"));

	GameplayTags.GameplayCue_Ingredient_JamPetrify = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.Ingredient.JamPetrify"),
	FString("Ingredient Jam Petrify Cue"));

	GameplayTags.GameplayCue_Ingredient_InProcessOfJamPetrify = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.Ingredient.InProcessOfJamPetrify"),
	FString("Ingredient In Process of Jam Petrify Cue"));

	GameplayTags.GameplayCue_Weapon_Change = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.Weapon.Change"),
	FString("Weapon Change GameplayCue"));

	GameplayTags.GameplayCue_Range_Change = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.Range.Change"),
	FString("Range Change GameplayCue"));

	GameplayTags.GameplayCue_Ingredient_Change = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.Ingredient.Change"),
	FString("Ingredient Change GameplayCue"));

	GameplayTags.GameplayCue_Recipe_Completed = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.Recipe.Completed"),
	FString("Recipe Completed GameplayCue"));

	GameplayTags.GameplayCue_QTE_QTEStart = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.QTE.QTEStart"),
	FString("QTE Start GameplayCue"));

	GameplayTags.GameplayCue_QTE_QTESelection = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.QTE.QTESelection"),
	FString("QTE Selection GameplayCue"));

	GameplayTags.GameplayCue_QTE_QTEFailedInput = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.QTE.QTEFailedInput"),
	FString("QTE Failed Input GameplayCue"));

	GameplayTags.GameplayCue_QTE_HitSound1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.QTE.HitSound1"),
	FString("QTE Hit Sound 1 GameplayCue"));

	GameplayTags.GameplayCue_QTE_HitSound2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.QTE.HitSound2"),
	FString("QTE Hit Sound 2 GameplayCue"));

	GameplayTags.GameplayCue_QTE_OnHitBloodVFX = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.QTE.OnHitBloodVFX"),
	FString("QTE On Hit Blood VFX GameplayCue"));

	GameplayTags.GameplayCue_QTE_FinisherBloodVFX = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.QTE.FinisherBloodVFX"),
	FString("QTE Finisher Blood VFX GameplayCue"));

	GameplayTags.GameplayCue_PlayerShieldBroken = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.PlayerShieldBroken"),
	FString("Player Shield Broken Cue"));

	GameplayTags.GameplayCue_Enemy_BoilVFX = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.Enemy.BoilVFX"),
FString("On Enemy Boil Hit Cue"));

	GameplayTags.GameplayCue_Enemy_GrillVFX = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("GameplayCue.Enemy.GrillVFX"),
FString("On Enemy Grill Hit Cue"));


	//----------------------------------------------------------------------------------------------------------------------------------
	//-------------------------------------Enemy Names Tags-----------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------------------------------------
	
	GameplayTags.Enemy_Gamusino = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Enemy.Gamusino"),
	FString("Enemy Name Gamusino"));

	GameplayTags.Enemy_Gamusino_Tutorial = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Enemy.Gamusino.Tutorial"),
	FString("Enemy Name Gamusino Tutorial"));

	GameplayTags.Enemy_Gamusino_Kami = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Enemy.Gamusino.Kami"),
	FString("Enemy Name Gamusino Kamikaze"));

	GameplayTags.Enemy_Gamusino_Muddy = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Enemy.Gamusino.Muddy"),
	FString("Enemy Name Gamusino Muddy"));

	GameplayTags.Enemy_Gamusino_Yeti = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Enemy.Gamusino.Yeti"),
	FString("Enemy Name Gamusino Yeti"));

	GameplayTags.Enemy_Gamusino_Premium = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Enemy.Gamusino.Premium"),
	FString("Enemy Name Gamusino Premium"));

	GameplayTags.Enemy_Cucafera = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Enemy.Cucafera"),
	FString("Enemy Name Cucafera"));

	GameplayTags.Enemy_Cucafera_Spectral = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Enemy.Cucafera.Spectral"),
	FString("Enemy Name Cucafera Spectral"));

	GameplayTags.Enemy_Cucafera_Guardian = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Enemy.Cucafera.Guardian"),
	FString("Enemy Name Cucafera Guardian"));

	GameplayTags.Enemy_Cucafera_Jumper = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Enemy.Cucafera.Jumper"),
	FString("Enemy Name Cucafera Jumper"));
	
	GameplayTags.Enemy_Cuelebre = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Enemy.Cuelebre"),
	FString("Enemy Name Cuelebre"));

	GameplayTags.Enemy_Basilisco = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Enemy.Basilisco"),
	FString("Enemy Name Basilisco"));

	GameplayTags.Enemy_Basilisco_Brute = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Enemy.Basilisco.Brute"),
	FString("Enemy Name Basilisco Brute"));

	GameplayTags.Enemy_Basilisco_Premium = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Enemy.Basilisco.Premium"),
	FString("Enemy Name Basilisco Premium"));

	GameplayTags.Enemy_Basilisco_Toxic = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Enemy.Basilisco.Toxic"),
	FString("Enemy Name Basilisco Toxic"));


	//----------------------------------------------------------------------------------------------------------------------------------
	//-------------------------------------Identify Effects Tags------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------------------------------------
	
	GameplayTags.Modify_Speed = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Modify.Speed"),
	FString("Identify ModifySpeed Effect"));

	
	//----------------------------------------------------------------------------------------------------------------------------------
	//-------------------------------------Type Tags------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------------------------------------
	
	GameplayTags.Type_State = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Type.State"),
	FString("Type State"));

	GameplayTags.Type_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Type.Attack"),
	FString("Type Attack"));

	//----------------------------------------------------------------------------------------------------------------------------------
	//-------------------------------------Notification Tags------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------------------------------------
	GameplayTags.Notification_FullInventory = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Notification.FullInventory"),
	FString("Notification when cant take anymore ammo of anytype"));
	GameplayTags.Notification_OutOfGrillAmmo = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Notification.OutOfGrillAmmo"),
	FString("Notification when Grill Ammo is low"));
	GameplayTags.Notification_OutOfBoilAmmo = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Notification.OutOfBoilAmmo"),
	FString("Notification when Boil Ammo is low"));
	GameplayTags.Notification_OutOfFryAmmo = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Notification.OutOfFryAmmo"),
	FString("Notification when Fry Ammo is low"));
	GameplayTags.Notification_OutOfCheeseAmmo = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Notification.OutOfCheeseAmmo"),
	FString("Notification when Cheese Ammo is low"));
	GameplayTags.Notification_OutOfJamAmmo = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Notification.OutOfJamAmmo"),
	FString("Notification when Jam Ammo is low"));
	GameplayTags.Notification_OutOfWineAmmo = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Notification.OutOfWineAmmo"),
	FString("Notification when Wine Ammo is low"));
	GameplayTags.Notification_Poison = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Notification.Poison"),
	FString("Notification when player is getting poisoned"));
	GameplayTags.Notification_Invisible = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Notification.Invisible"),
	FString("Notification when player is invisible to enemies"));
	GameplayTags.Notification_NeedAnotherMelee = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Notification.NeedAnotherMelee"),
	FString("Notification when player needs another melee weapon tier"));
	
}

