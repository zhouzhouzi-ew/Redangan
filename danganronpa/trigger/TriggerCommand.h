#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TriggerCommand.generated.h"

class AMyTriggerBox;
class AActor;

/**
 * Refactored with Command Pattern:
 * Encapsulates what should happen when a trigger box is hit.
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class DANGANRONPA_API UTriggerCommand : public UObject
{
    GENERATED_BODY()

public:
    /** Execute the command with the trigger as context. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Trigger|Command")
    void Execute(AMyTriggerBox* Trigger, AActor* OtherActor);

    virtual void Execute_Implementation(AMyTriggerBox* Trigger, AActor* OtherActor);
};
