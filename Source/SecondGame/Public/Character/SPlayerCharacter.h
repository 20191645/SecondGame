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

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	// 'IA_Move' �Է¿� �����ϴ� �Լ�
	void InputMove(const FInputActionValue& InValue);
	// 'IA_Look' �Է¿� �����ϴ� �Լ�
	void InputLook(const FInputActionValue& InValue);

	// 'IA_QuickSlot01' �Է¿� �����ϴ� �Լ�
	void InputQuickSlot01(const FInputActionValue& InValue);
	// 'IA_QuickSlot02' �Է¿� �����ϴ� �Լ�
	void InputQuickSlot02(const FInputActionValue& InValue);
	// 'IA_QuickSlot03' �Է¿� �����ϴ� �Լ�
	void InputQuickSlot03(const FInputActionValue& InValue);

	// 'IA_Crouch' �Է¿� �����ϴ� �Լ�
	void InputCrouch(const FInputActionValue& InValue);

	// 'IA_Attack' �Է¿� �����ϴ� �Լ�
	void InputAttack(const FInputActionValue& InValue);

	// �ܹ� ��� �Լ�
	void TryFire();

	// 'IA_Zoom' �Է� ������ �����ϴ� ���� �Լ�
	void ZoomIn(const FInputActionValue& InValue);
	// 'IA_Zoom' �Է� ���� �����ϴ� ���� �Լ�
	void ZoomOut(const FInputActionValue& InValue);

	// 'IA_Trigger' �Է¿� �����ϴ� �Լ�
	void ToggleTrigger(const FInputActionValue& InValue);
	// 'IA_Attack' �Է� ������ �����ϴ� �Լ�
	void StartFire(const FInputActionValue& InValue);
	// 'IA_Attack' �Է� ���� �����ϴ� �Լ�
	void StopFire(const FInputActionValue& InValue);

protected:
	// SpringArmComponent: 3��Ī ���� ī�޶� ���� ���� ���� ������Ʈ - ī�޶� �� ����, ������Ʈ ȸ�� ����
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
	float FirePerMinute = 600;
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
};
