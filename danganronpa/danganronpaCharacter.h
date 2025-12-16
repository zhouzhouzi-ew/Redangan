// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "danganronpaCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class DANGANRONPA_API ADanganronpaCharacter : public ACharacter
{
GENERATED_BODY()

public:
ADanganronpaCharacter();

/** 返回第一人称手臂网格 */
UFUNCTION(BlueprintPure, Category = "Mesh")
USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

/** 返回摄像机 */
UFUNCTION(BlueprintPure, Category = "Camera")
UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

/** 死亡函数（可被触发器调用） */
UFUNCTION(BlueprintCallable, Category = "Gameplay")
void Die();

protected:
virtual void BeginPlay() override;
virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

void Move(const FInputActionValue& Value);
void Look(const FInputActionValue& Value);

private:
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
USkeletalMeshComponent* Mesh1P;

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
UCameraComponent* FirstPersonCameraComponent;

UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
UInputAction* JumpAction;

UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
UInputAction* MoveAction;

UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
UInputAction* LookAction;
};