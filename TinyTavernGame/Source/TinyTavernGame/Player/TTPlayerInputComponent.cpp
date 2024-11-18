// TinyTavern


#include "TTPlayerInputComponent.h"

#include "TTPlayerInputConfig.h"


// Sets default values for this component's properties
UTTPlayerInputComponent::UTTPlayerInputComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTTPlayerInputComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTTPlayerInputComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}
template <class UserClass, typename PressedFuncType, typename HeldFuncType, typename ReleasedFuncType>
void UTTPlayerInputComponent::BindAbilityActions(const UTTPlayerInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, HeldFuncType HeldFunc, ReleasedFuncType ReleasedFunc)
{
	check(InputConfig);
	for(const FTTInputAction& Action : InputConfig->AbilityInputActions)
	{
		if(Action.InputAction && Action.InputTag.IsValid())
		{
			if(PressedFunc)
			{
				BindAction(Action.InputAction,ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}
			if(HeldFunc)
			{
				BindAction(Action.InputAction,ETriggerEvent::Triggered, Object,HeldFunc, Action.InputTag);
			}
			if(ReleasedFunc)
			{
				BindAction(Action.InputAction,ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);	
			}
		}
	}
}

