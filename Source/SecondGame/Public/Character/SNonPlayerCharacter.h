// SNonPlayerCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "Character/SCharacter.h"
#include "SNonPlayerCharacter.generated.h"

class ASWeaponActor;
class USBW_HPBar;
class USWidgetComponent;

UCLASS()
class SECONDGAME_API ASNonPlayerCharacter : public ASCharacter
{
	GENERATED_BODY()
	
	friend class UBTTask_Attack;
	friend class UBTTask_Reload;

public:
	ASNonPlayerCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void SetHPBarWidget(USBW_HPBar* HPBarWidget);

protected:
	// �ܹ� ��� �Լ�
	void TryFire();
	// ���� ��� �Լ�
	void StartFire();

	// 'OnFireEffect' ��������Ʈ�� ���ε��� �Լ�
	UFUNCTION()
	void OnFireEffect();

	// �Ѿ� ���� �Լ�
	void Reload();

protected:
	// ���� Ŭ���� ���� -- 'Assault_Rifle_A'
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSubclassOf<ASWeaponActor> WeaponClass;
	// ���� ���� ��ü
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<ASWeaponActor> WeaponInstance;

	// �д� ��� Ƚ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	float FirePerMinute = 75;
	// ���� ��� Ÿ�̸�
	FTimerHandle BetweenShotsTimer;
	// ���� ��� �ð� ����
	float TimeBetweenFire;

	// �ִ� �Ѿ� ����
	int32 MaxBulletCount;

	// HP�� ���� ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<USWidgetComponent> WidgetComponent;
};
