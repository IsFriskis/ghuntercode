// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TT_TimeLine.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimeLine,float,Value);
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UTT_TimeLine : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	// Constructor
	UTT_TimeLine();

	// Nodo de Blueprint que se llama para iniciar la tarea
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
	static UTT_TimeLine* TT_TimeLine(UObject* WorldContextObject,FName TaskInstanceName,float BlendTime,bool Reversed, EAlphaBlendOption BlendOption, float StartValue, float EndValue);

	// Se ejecuta cuando la tarea finaliza
	UPROPERTY(BlueprintAssignable)
	FTimeLine OnCompleted;

	// Función que se llama para iniciar la tarea
	void Activate() override;

	UFUNCTION()
	void Finish();
	

	UFUNCTION()
	virtual void Update();
	
	UPROPERTY()
	float BlendTime = 1;

	UPROPERTY()
	bool Reversed = false;

	UPROPERTY()
	EAlphaBlendOption BlendOption = EAlphaBlendOption::Linear;

	UPROPERTY()
	float StartValue = 1;

	UPROPERTY()
	float EndValue = 0;

	FAlphaBlend AlphaBlend;
	float Timer = 0;

	TObjectPtr<UObject> WorldContextObject;

private:
	// Función simulada para la tarea asincrónica
	void DoWork();
	
};
