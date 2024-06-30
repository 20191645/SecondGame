// SWeaponActor.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeaponActor.generated.h"

class UAnimInstance;
class UAnimMontage;
class UParticleSystem;

UCLASS()
class SECONDGAME_API ASWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASWeaponActor();

	// 'Mesh' Getter �Լ�
	USkeletalMeshComponent* GetMesh() const { return Mesh; }

	// 'PistolCharacterAnimLayer, RifleCharacterAnimLayer' Getter �Լ�
	TSubclassOf<UAnimInstance> GetPistolCharacterAnimLayer() const { return PistolCharacterAnimLayer; }
	TSubclassOf<UAnimInstance> GetRifleCharacterAnimLayer() const { return RifleCharacterAnimLayer; }

	// 'EquipAnimMontage' Getter �Լ�
	UAnimMontage* GetEquipAnimMontage() const { return EquipAnimMontage; }

	// 'MaxRange' Getter �Լ�
	float GetMaxRange() const { return MaxRange; }

	// 'FireAnimMontage' Getter �Լ�
	UAnimMontage* GetFireAnimMontage() const { return FireAnimMontage; }

	// 'FireParticleSystem' Getter �Լ�
	UParticleSystem* GetFireParticleSystem() const { return FireParticleSystem; }

	// 'ReloadAnimMontage' Getter �Լ�
	UAnimMontage* GetReloadAnimMontage() const { return ReloadAnimMontage; }

protected:
	// ���� �������� ����� Mesh ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<USkeletalMeshComponent> Mesh;

	// �ǽ��� ���� ���� �� ����� Animation Layer
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASWeaponActor|AnimLayer", meta = (AllowPrivateAccess))
	TSubclassOf<UAnimInstance> PistolCharacterAnimLayer;
	// ������ ���� ���� �� ����� Animation Layer
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASWeaponActor|AnimLayer", meta = (AllowPrivateAccess))
	TSubclassOf<UAnimInstance> RifleCharacterAnimLayer;

	// ���� ���� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> EquipAnimMontage;

	// ���� �ִ� ��Ÿ� (����: cm)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess, Units = cm))
	float MaxRange = 25000.f;

	// ��� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> FireAnimMontage;

	// ��� ȿ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UParticleSystem> FireParticleSystem;

	// �Ѿ� ���� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> ReloadAnimMontage;
};
