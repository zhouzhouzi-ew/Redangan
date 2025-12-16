#include "OpenLevelCommand.h"
#include "Kismet/GameplayStatics.h"

void UOpenLevelCommand::Execute_Implementation(AMyTriggerBox* Trigger, AActor* OtherActor)
{
    if (!Trigger)
    {
        return;
    }

    if (!LevelName.IsNone())
    {
        UGameplayStatics::OpenLevel(Trigger, LevelName);
    }
}
