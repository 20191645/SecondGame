// SCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USStatComponent;
class USBW_Bar;
class UAnimMontage;

UCLASS()
class SECONDGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASCharacter();

	// 'StatComponent' Getter �Լ�
	USStatComponent* GetStatComponent() const { return StatComponent; }

	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// 'CurrentAimPitch, CurrentAimYaw' Getter �Լ�
	float GetCurrentAimPitch() const { return CurrentAimPitch; }
	float GetCurrentAimYaw() const { return CurrentAimYaw; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	virtual void OnCharacterDeath();

protected:
	// ĳ���� ���� ����(HP)�� �����ϴ� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<USStatComponent> StatComponent;

	// ���� Pitch, Yaw ���� ���� ��
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float CurrentAimPitch = 0.f;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float CurrentAimYaw = 0.f;

	// ���� Pitch, Yaw ���� ���� ��
	float PreviousAimPitch = 0.f;
	float PreviousAimYaw = 0.f;

	// �ǰ� �ִϸ��̼�
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> HitReactAnimMontage;
};
