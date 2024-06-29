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
};
