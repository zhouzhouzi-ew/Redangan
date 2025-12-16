#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "WeaponComponent.generated.h"

class ADanganronpaCharacter;
class ADanganronpaProjectile;
class UInputMappingContext;
class UInputAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DANGANRONPA_API UWeaponComponent : public USkeletalMeshComponent
{
GENERATED_BODY()

public:
UWeaponComponent();

/** Projectile class to spawn */
UPROPERTY(EditDefaultsOnly, Category = "Weapon")
TSubclassOf<ADanganronpaProjectile> ProjectileClass;

/** Sound to play on fire */
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
USoundBase* FireSound;

/** Animation montage to play on fire (1P) */
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
UAnimMontage* FireAnimation;

/** Muzzle offset from camera */
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
FVector MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);

/** Input Mapping Context for fire action */
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
UInputMappingContext* FireMappingContext;

/** Fire Input Action */
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
UInputAction* FireAction;

UFUNCTION(BlueprintCallable, Category = "Weapon")
bool AttachToCharacter(ADanganronpaCharacter* TargetCharacter);

UFUNCTION(BlueprintCallable, Category = "Weapon")
void Fire();

protected:
virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
UPROPERTY()
ADanganronpaCharacter* OwningCharacter = nullptr;
};