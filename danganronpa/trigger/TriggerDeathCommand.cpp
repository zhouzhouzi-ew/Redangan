#include "TriggerDeathCommand.h"
#include "MyTriggerBox.h"

void UTriggerDeathCommand::Execute_Implementation(AMyTriggerBox* Trigger, AActor* OtherActor)
{
    if (Trigger)
    {
        Trigger->TriggerDeath();
    }
}
