// SPlayerCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "Character/SCharacter.h"
#include "InputActionValue.h"
#include "SPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USInputConfigData;
class UInputMappingContext;
class ASWeaponActor;
class UCameraShakeBase;
class UParticleSystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrentBulletCountChangeDelegate, int32, InWeaponClassNumber, int32, InCurrentBulletCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponClassNumberChangeDelegate, int32, InWeaponClassNumber);

UCLASS()
class SECONDGAME_API ASPlayerCharacter : public ASCharacter
{
	GENERATED_BODY()

public:
	ASPlayerCharacter();

	virtual void BeginPlay() override;

	// Input Value �Ӽ�(Forward, Right) Getter �Լ�
	float GetForwardInputValue() const { return ForwardInputValue; }
	float GetRightInputValue() const { return RightInputValue; }

	virtual void Tick(float DeltaSeconds) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// 'CurrentBulletCount' Getter, Setter �Լ�
	int32 GetCurrentBulletCount(int32 InWeaponClassNumber) const { return CurrentBulletCount[InWeaponClassNumber - 1]; }
	void SetCurrentBulletCount(int32 InWeaponClassNumber, int32 InCurrentBulletCount);
	// 'MaxBulletCount' Getter, Setter �Լ�
	int32 GetMaxBulletCount(int32 InWeaponClassNumber) const { return MaxBulletCount[InWeaponClassNumber - 1]; }

	// 'WeaponClassNumber' Getter, Setter �Լ�
	int32 GetWeaponClassNumber() const { return WeaponClassNumber; }
	void SetWeaponClassNumber(int32 InWeaponClassNumber);

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// ĳ���� ������ ȣ��Ǵ� �Լ�
	virtual void OnCharacterDeath() override;

	// 'OnFireEffect' ��������Ʈ�� ���ε��� �Լ�
	UFUNCTION()
	void OnFireEffect();

	// ĳ���� �������ϴ� �Լ�
	void Respawn();

private:
	// 'IA_Move' �Է¿� �����ϴ� �Լ�
	void InputMove(const FInputActionValue& InValue);
	// 'IA_Look' �Է¿� �����ϴ� �Լ�
	void InputLook(const FInputActionValue& InValue);

	// 'IA_QuickSlot01' �Է¿� �����ϴ� �Լ�
	void InputQuickSlot01();
	// 'IA_QuickSlot02' �Է¿� �����ϴ� �Լ�
	void InputQuickSlot02();
	// 'IA_QuickSlot03' �Է¿� �����ϴ� �Լ�
	void InputQuickSlot03();

	// 'IA_Crouch' �Է¿� �����ϴ� �Լ�
	void InputCrouch();

	// 'IA_Attack' �Է¿� �����ϴ� �Լ�
	void InputAttack();

	// �ܹ� ��� �Լ�
	void TryFire();

	// 'IA_Zoom' �Է� ������ �����ϴ� ���� �Լ�
	void ZoomIn();
	// 'IA_Zoom' �Է� ���� �����ϴ� ���� �Լ�
	void ZoomOut();

	// 'IA_Trigger' �Է¿� �����ϴ� �Լ�
	void ToggleTrigger();
	// 'IA_Attack' �Է� ������ �����ϴ� �Լ�
	void StartFire();
	// 'IA_Attack' �Է� ���� �����ϴ� �Լ�
	void StopFire();

	// 'IA_Reload' �Է¿� �����ϴ� �Լ�
	void InputReload();

public:
	// 'CurrentBulletCount'�� ��ȭ�ϸ� BroadCast�ϴ� ��������Ʈ
	FOnCurrentBulletCountChangeDelegate OnCurrentBulletCountChangedDelegate;

	// 'WeaponClassNumber'�� ��ȭ�ϸ� BroadCast�ϴ� ��������Ʈ
	FOnWeaponClassNumberChangeDelegate OnWeaponClassNumberChangedDelegate;

protected:
	// SpringArmComponent: 3��Ī ���� ī�޶� ���� ���� ���� ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	// CameraComponenet: ���� ���� ����� �÷��̾� ȭ�鿡 �����ϴ� ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UCameraComponent> CameraComponent;

	// 'IC_PlayerCharacter'�� ���ε��� �Ӽ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<USInputConfigData> PlayerCharacterInputConfigData;
	// 'IMC_PlayerCharacter'�� ���ε��� ������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UInputMappingContext> PlayerCharacterInputMappingContext;
	
	// �ִϸ��̼�(AnimInstance)�� ���ؼ� Input Value �Ӽ�(Forward, Right) �߰�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float ForwardInputValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float RightInputValue;

	// ���� ����1 Ŭ���� ���� -- 'Pistols_A' -- ��X, ����X
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSubclassOf<ASWeaponActor> WeaponClass01;
	// ���� ����2 Ŭ���� ���� -- 'Assault_Rifle_A' -- ��X, ����O
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSubclassOf<ASWeaponActor> WeaponClass02;
	// ���� ����3 Ŭ���� ���� -- 'Sniper_Rifle_A' -- ��O, ����X
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSubclassOf<ASWeaponActor> WeaponClass03;
	// ���� ���� ��ü
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<ASWeaponActor> WeaponInstance;

	// ��ǥ FOV ��
	float TargetFOV = 70.f;
	// ���� FOV ��
	float CurrentFOV = 70.f;

	// ���� ��� �� �д� ��� Ƚ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	float FirePerMinute = 360;
	// 'IA_Trigger' �Է� �� �� ���� -- �ܹ�/���� ��۸�
	bool bIsTriggerToggle = false;
	// ���� ��� Ÿ�̸�
	FTimerHandle BetweenShotsTimer;
	// ���� ��� �ð� ����
	float TimeBetweenFire;

	// �� ���� Ŭ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> SniperZoomUIClass;
	// �� ���� ��ü
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UUserWidget> SniperZoomUIInstance;

	// ���� ���� Ŭ���� ��ȣ
	int32 WeaponClassNumber;

	// ī�޶� ��鸲 Ŭ���� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<UCameraShakeBase> FireCameraShake;

	// ���⺰ ���� �Ѿ� ����
	TArray<int32> CurrentBulletCount;
	// ���⺰ �ִ� �Ѿ� ����
	TArray<int32> MaxBulletCount;

	// ĳ���� ��Ȱ ����Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UParticleSystemComponent> RespawnParticleSystemComponent;

	// ��� ���� ȿ�� ���� Ŭ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> FireHitEffectUIClass;
	// ��� ���� ȿ�� ���� ��ü
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UUserWidget> FireHitEffectUIInstance;
};
