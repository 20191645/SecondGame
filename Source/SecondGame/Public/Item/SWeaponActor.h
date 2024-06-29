// SWeaponActor.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeaponActor.generated.h"

class UAnimInstance;
class UAnimMontage;

UCLASS()
class SECONDGAME_API ASWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASWeaponActor();

	// 'Mesh' Getter 함수
	USkeletalMeshComponent* GetMesh() const { return Mesh; }

	// 'PistolCharacterAnimLayer, RifleCharacterAnimLayer' Getter 함수
	TSubclassOf<UAnimInstance> GetPistolCharacterAnimLayer() const { return PistolCharacterAnimLayer; }
	TSubclassOf<UAnimInstance> GetRifleCharacterAnimLayer() const { return RifleCharacterAnimLayer; }

	// 'EquipAnimMontage' Getter 함수
	UAnimMontage* GetEquipAnimMontage() const { return EquipAnimMontage; }

	// 'MaxRange' Getter 함수
	float GetMaxRange() const { return MaxRange; }

	// 'FireAnimMontage' Getter 함수
	UAnimMontage* GetFireAnimMontage() const { return FireAnimMontage; }

protected:
	// 무기 아이템이 사용할 Mesh 정보
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<USkeletalMeshComponent> Mesh;

	// 피스톨 무기 장착 시 사용할 Animation Layer
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASWeaponActor|AnimLayer", meta = (AllowPrivateAccess))
	TSubclassOf<UAnimInstance> PistolCharacterAnimLayer;
	// 라이플 무기 장착 시 사용할 Animation Layer
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASWeaponActor|AnimLayer", meta = (AllowPrivateAccess))
	TSubclassOf<UAnimInstance> RifleCharacterAnimLayer;

	// 무기 장착 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> EquipAnimMontage;

	// 공격 최대 사거리 (단위: cm)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess, Units = cm))
	float MaxRange = 25000.f;

	// 사격 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> FireAnimMontage;
};
