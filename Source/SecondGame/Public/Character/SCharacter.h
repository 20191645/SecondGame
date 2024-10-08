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

	// 'StatComponent' Getter 함수
	USStatComponent* GetStatComponent() const { return StatComponent; }

	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// 'CurrentAimPitch, CurrentAimYaw' Getter 함수
	float GetCurrentAimPitch() const { return CurrentAimPitch; }
	float GetCurrentAimYaw() const { return CurrentAimYaw; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	virtual void OnCharacterDeath();

protected:
	// 캐릭터 스탯 정보(HP)를 관리하는 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<USStatComponent> StatComponent;

	// 현재 Pitch, Yaw 방향 에임 값
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float CurrentAimPitch = 0.f;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float CurrentAimYaw = 0.f;

	// 이전 Pitch, Yaw 방향 에임 값
	float PreviousAimPitch = 0.f;
	float PreviousAimYaw = 0.f;

	// 피격 애니메이션
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> HitReactAnimMontage;
};
