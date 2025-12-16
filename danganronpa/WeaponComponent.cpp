#include "Weapons/WeaponComponent.h"
#include "Characters/DanganronpaCharacter.h"
#include "Weapons/Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"

UWeaponComponent::UWeaponComponent()
{
    // 默认值已在头文件中设置
}

bool UWeaponComponent::AttachToCharacter(ADanganronpaCharacter* TargetCharacter)
{
    if (!TargetCharacter || TargetCharacter->FindComponentByClass<UWeaponComponent>())
    {
        return false;
    }

    OwningCharacter = TargetCharacter;

    // 附加到第一人称手臂的 GripPoint socket
    FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);
    AttachToComponent(OwningCharacter->GetMesh1P(), Rules, TEXT("GripPoint"));

    // 注册为实例组件（便于保存）
    OwningCharacter->AddInstanceComponent(this);

    // 设置输入绑定
    if (APlayerController* PC = Cast<APlayerController>(OwningCharacter->GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(FireMappingContext, 1);
        }

        if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PC->InputComponent))
        {
            EIC->BindAction(FireAction, ETriggerEvent::Triggered, this, &UWeaponComponent::Fire);
        }
    }

    return true;
}

void UWeaponComponent::Fire()
{
    if (!OwningCharacter || !OwningCharacter->GetController())
    {
        return;
    }

    UWorld* World = GetWorld();
    if (!World || !ProjectileClass)
    {
        return;
    }

    APlayerController* PC = Cast<APlayerController>(OwningCharacter->GetController());
    if (!PC || !PC->PlayerCameraManager)
    {
        return;
    }

    const FRotator SpawnRotation = PC->PlayerCameraManager->GetCameraRotation();
    const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

    World->SpawnActor<ADanganronpaProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

    // 播放音效
    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, OwningCharacter->GetActorLocation());
    }

    // 播放第一人称开火动画
    if (FireAnimation && OwningCharacter->GetMesh1P())
    {
        if (UAnimInstance* AnimInstance = OwningCharacter->GetMesh1P()->GetAnimInstance())
        {
            AnimInstance->Montage_Play(FireAnimation, 1.0f);
        }
    }
}

void UWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (OwningCharacter && FireMappingContext)
    {
        if (APlayerController* PC = Cast<APlayerController>(OwningCharacter->GetController()))
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
            {
                Subsystem->RemoveMappingContext(FireMappingContext);
            }
        }
    }

    Super::EndPlay(EndPlayReason);
}