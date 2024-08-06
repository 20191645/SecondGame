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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// ĳ���� ������ ȣ��Ǵ� �Լ�
	virtual void OnCharacterDeath() override;

	// 'OnFireEffect' ��������Ʈ�� ���ε��� �Լ� -- Notify('FireEffect')�� ����
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

	// 'IA_Manual' �Է¿� �����ϴ� �Լ�
	void InputManual();

	// 'WeaponInstacne' ���� 
	// -- Server: �������� ����
	UFUNCTION(Server, Reliable)
	void SpawnWeaponInstance_Server();
	// 'WeaponInstance' �ı�
	UFUNCTION(Server, Reliable)
	void DestroyWeaponInstance_Server();

	// 'WeaponInstance'�� ������ �� ȣ��Ǵ� �Լ�
	UFUNCTION()
	void OnRep_WeaponInstance();

	// 'WeaponClassNumber' ����
	UFUNCTION(Server, Reliable)
	void SetWeaponClassNumber_Server(int32 InWeaponClassNumber);


	// ������ ����Ʈ Ȱ��ȭ �Լ� -- ���� ���ٿ�
	UFUNCTION(Server, Unreliable)
	void RespawnEffect_Server();
	// ������ ����Ʈ Ȱ��ȭ �Լ� 
	// -- NetMulticast: �������� ȣ��, ��� PC���� ����
	UFUNCTION(NetMulticast, Unreliable)
	void RespawnEffect_NetMulticast();

	// ��� ����Ʈ Ȱ��ȭ �Լ� -- ���� ���ٿ�
	UFUNCTION(Server, Unreliable)
	void FireEffect_Server();
	// ��� ����Ʈ Ȱ��ȭ �Լ�
	UFUNCTION(NetMulticast, Unreliable)
	void FireEffect_NetMulticast();

	// 'ForwardInputValue, RightInputValue' ������Ʈ
	UFUNCTION(Server, Unreliable)
	void UpdateInputValue_Server(const float& InForwardInputValue, const float& InRightInputValue);
	// 'CurrentAimPitch, CurrentAimYaw' ������Ʈ
	UFUNCTION(Server, Unreliable)
	void UpdateAimValue_Server(const float& InAimPitchValue, const float& InAimYawValue);

	// �Ѿ� ���� �ִϸ��̼� ��� �Լ� -- ���� ���ٿ�
	UFUNCTION(Server, Unreliable)
	void PlayReloadMontage_Server();
	// �Ѿ� ���� �ִϸ��̼� ��� �Լ� -- Other Client
	UFUNCTION(NetMulticast, Unreliable)
	void PlayReloadMontage_NetMulticast();

	// ��� �ִϸ��̼� ��� �Լ� -- ���� ���ٿ�
	UFUNCTION(Server, Unreliable)
	void PlayFireMontage_Server();
	// ��� �ִϸ��̼� ��� �Լ� -- Other Client
	UFUNCTION(NetMulticast, Unreliable)
	void PlayFireMontage_NetMulticast();

	// ������ ó�� �Լ�
	UFUNCTION(Server, Reliable)
	void ApplyDamage_Server(FHitResult HitResult, float HitDamage);

	// �ǰ� �ִϸ��̼� ��� �Լ� -- Owner, Other Client
	UFUNCTION(NetMulticast, Reliable)
	void PlayHitReactMontage_NetMulticast();

	// �÷��̾� ĳ���� �ı� �Լ� -- ���� ���ٿ�
	UFUNCTION(Server, Reliable)
	void DestroyPlayerCharacter_Server();
	// �÷��̾� ĳ���� �ı� �Լ� -- Other Client
	UFUNCTION(NetMulticast, Reliable)
	void DestroyPlayerCharacter_NetMulticast();

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
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float ForwardInputValue;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float RightInputValue;

	// ���� Input Value �Ӽ� ��
	float PreviousForwardInputValue = 0.f;
	float PreviousRightInputValue = 0.f;

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
	UPROPERTY(ReplicatedUsing = OnRep_WeaponInstance, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
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
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
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
