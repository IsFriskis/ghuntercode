// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "TT_AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCookbarTypeChanged, FGameplayTag, CookType);

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}
	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>  SourceASC = nullptr;

	UPROPERTY()
	TObjectPtr<AActor>	SourceAvatarActor = nullptr;

	UPROPERTY()
	TObjectPtr<AController> SourceController = nullptr;

	UPROPERTY()
	TObjectPtr<ACharacter> SourceCharacter = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetASC = nullptr;

	UPROPERTY()
	TObjectPtr<AActor> TargetAvatarActor = nullptr;

	UPROPERTY()
	TObjectPtr<AController> TargetController = nullptr;

	UPROPERTY()
	TObjectPtr<ACharacter> TargetCharacter = nullptr;
	
};

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UTT_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData &Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnCookbarTypeChanged OnCookbarTypeChanged;

	//Attribute declaration
	UPROPERTY(BlueprintReadWrite, Category = "Vitals" )
	FGameplayAttributeData MaxSpeed;
	ATTRIBUTE_ACCESSORS(UTT_AttributeSet, MaxSpeed);

	UPROPERTY(BlueprintReadWrite, Category = "Vitals")
	FGameplayAttributeData Speed;
	ATTRIBUTE_ACCESSORS(UTT_AttributeSet, Speed);

	UPROPERTY(BlueprintReadWrite, Category = "Vitals")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UTT_AttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadWrite, Category = "Vitals")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UTT_AttributeSet, Health)

	UPROPERTY(BlueprintReadWrite, Category = "Vitals")
	FGameplayAttributeData MaxCookbar;
	ATTRIBUTE_ACCESSORS(UTT_AttributeSet, MaxCookbar)

	UPROPERTY(BlueprintReadWrite, Category = "Vitals")
	FGameplayAttributeData Cookbar;
	ATTRIBUTE_ACCESSORS(UTT_AttributeSet, Cookbar)

	UPROPERTY(BlueprintReadWrite, Category = "Vitals")
	FGameplayAttributeData MaxPoisonResistance;
	ATTRIBUTE_ACCESSORS(UTT_AttributeSet, MaxPoisonResistance)

	UPROPERTY(BlueprintReadWrite, Category = "Vitals")
	FGameplayAttributeData PoisonResistance;
	ATTRIBUTE_ACCESSORS(UTT_AttributeSet, PoisonResistance)
	
	UPROPERTY(BlueprintReadWrite, Category = "Ammo")
	FGameplayAttributeData MaxWineAmmo;
	ATTRIBUTE_ACCESSORS(UTT_AttributeSet, MaxWineAmmo)

	UPROPERTY(BlueprintReadWrite, Category = "Ammo")
	FGameplayAttributeData WineAmmo;
	ATTRIBUTE_ACCESSORS(UTT_AttributeSet,WineAmmo)

	UPROPERTY(BlueprintReadWrite, Category = "Ammo")
	FGameplayAttributeData MaxCheeseAmmo;
	ATTRIBUTE_ACCESSORS(UTT_AttributeSet, MaxCheeseAmmo)

	UPROPERTY(BlueprintReadWrite, Category = "Ammo")
	FGameplayAttributeData CheeseAmmo;
	ATTRIBUTE_ACCESSORS(UTT_AttributeSet,CheeseAmmo)
	
	UPROPERTY(BlueprintReadWrite, Category = "Ammo")
	FGameplayAttributeData MaxJamAmmo;
	ATTRIBUTE_ACCESSORS(UTT_AttributeSet, MaxJamAmmo)

	UPROPERTY(BlueprintReadWrite, Category = "Ammo")
	FGameplayAttributeData JamAmmo;
	ATTRIBUTE_ACCESSORS(UTT_AttributeSet,JamAmmo)
	
	UPROPERTY(BlueprintReadWrite, Category = "Ammo")
	FGameplayAttributeData MaxGrilledAmmo;
	ATTRIBUTE_ACCESSORS(UTT_AttributeSet, MaxGrilledAmmo)
	
	UPROPERTY(BlueprintReadWrite, Category = "Ammo")
	FGameplayAttributeData GrilledAmmo;
	ATTRIBUTE_ACCESSORS(UTT_AttributeSet, GrilledAmmo);

	UPROPERTY(BlueprintReadWrite, Category = "Ammo")
	FGameplayAttributeData MaxBoiledAmmo;
	ATTRIBUTE_ACCESSORS(UTT_AttributeSet, MaxBoiledAmmo)

	UPROPERTY(BlueprintReadWrite, Category = "Ammo")
	FGameplayAttributeData BoiledAmmo;
	ATTRIBUTE_ACCESSORS(UTT_AttributeSet, BoiledAmmo)

	UPROPERTY(BlueprintReadWrite, Category = "Ammo")
	FGameplayAttributeData MaxFriedAmmo;
	ATTRIBUTE_ACCESSORS(UTT_AttributeSet, MaxFriedAmmo)

	UPROPERTY(BlueprintReadWrite, Category = "Ammo")
	FGameplayAttributeData FriedAmmo;
	ATTRIBUTE_ACCESSORS(UTT_AttributeSet, FriedAmmo)

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	FGameplayAttributeData MaxDamage;
	ATTRIBUTE_ACCESSORS(UTT_AttributeSet, MaxDamage)

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UTT_AttributeSet, Damage)

	
private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) ;

	bool LowHealth;
};

