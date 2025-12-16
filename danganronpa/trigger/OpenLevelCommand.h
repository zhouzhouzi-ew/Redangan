#pragma once

#include "CoreMinimal.h"
#include "TriggerCommand.h"
#include "OpenLevelCommand.generated.h"

/**
 * Refactored with Command Pattern:
 * Opens a specific level when the trigger is hit.
 */
UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class DANGANRONPA_API UOpenLevelCommand : public UTriggerCommand
{
    GENERATED_BODY()

public:
    /** Target level to open. Example: "dialogueEndStage" */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger|Command")
    FName LevelName;

    virtual void Execute_Implementation(AMyTriggerBox* Trigger, AActor* OtherActor) override;
};
