#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Engine/TriggerBox.h"
#include "MyTriggerBox.generated.h"

class UTriggerCommand;
class AActor;




UCLASS()
class DANGANRONPA_API AMyTriggerBox : public ATriggerBox
{

    GENERATED_BODY()

public:
    // 构造函数
    AMyTriggerBox();

protected:
    // 触发框
    UPROPERTY(VisibleAnywhere, Category = "Trigger")
    UBoxComponent* TriggerBox;

    // Refactored with Command Pattern: command to execute when hit
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Trigger|Command")
    TObjectPtr<UTriggerCommand> HitCommand;

    // Cache the actor that caused the trigger (optional context for commands)
    UPROPERTY(Transient)
    TObjectPtr<AActor> LastOtherActor;

    // 重叠事件函数
    UFUNCTION(BlueprintCallable)
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);

public:
    // 被触发时执行的事件
    UFUNCTION(BlueprintCallable)
    void OnTriggerHit();

    UFUNCTION(BlueprintCallable)
    void TriggerDeath();

    UFUNCTION(BlueprintCallable)
    void OpenNextLevel();
};

