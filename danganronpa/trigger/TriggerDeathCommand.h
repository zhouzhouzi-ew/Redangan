#pragma once

#include "CoreMinimal.h"
#include "TriggerCommand.h"
#include "TriggerDeathCommand.generated.h"

/**
 * Refactored with Command Pattern:
 * Kills the player (calls AMyTriggerBox::TriggerDeath()).
 */
UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class DANGANRONPA_API UTriggerDeathCommand : public UTriggerCommand
{
    GENERATED_BODY()

public:
    virtual void Execute_Implementation(AMyTriggerBox* Trigger, AActor* OtherActor) override;
};
