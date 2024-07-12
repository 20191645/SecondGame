// SAnimInstance.h

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SAnimInstance.generated.h"

class UAnimMontage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFireEffect);

// 'LocomotionSM'�� State
UENUM(BlueprintType)
enum class ELocomotionState : uint8
{
	None,
	Idle,	// ����
	Walk,	// �ȱ�
	End
};

// ĳ���Ͱ� �����̴� ����
UENUM(BlueprintType)
enum class EMovementDirection : uint8
{
	None,
	Fwd,	// ��
	Bwd,	// ��
	Left,	// ��
	Right,	// ��
	End
};

UCLASS()
class SECONDGAME_API USAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	USAnimInstance();

	// NativeInitializeAnimation(): �ʱ�ȭ �Լ�
	virtual void NativeInitializeAnimation() override;

	// NativeUpdateAnimation(): AnimInstance�� Tick() �Լ�
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// 'LocomotionState, MovementDirection' Getter �Լ�
	ELocomotionState GetLocomotionState() const { return LocomotionState; }
	EMovementDirection GetMovementDirection() const { return MovementDirection; }

	// 'bIsDead' Getter �Լ�
	uint8 GetbIsDead() const { return bIsDead; }

protected:
	// Notify 'FireEffect'�� ��ġ�� �����ӿ� ȣ���� �Լ�
	UFUNCTION()
	void AnimNotify_FireEffect();

public:
	// 'FOnFireEffect' ��������Ʈ
	FOnFireEffect OnFireEffect;

protected:
	// ���� ĳ������ �ӷ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SAnimInstance", meta = (AllowPrivateAccess))
	float CurrentSpeed;
	// ���� ĳ������ �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SAnimInstance", meta = (AllowPrivateAccess))
	FVector Velocity;

	// ĳ���Ͱ� ���� ���߿� ���ִ���
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SAnimInstance", meta = (AllowPrivateAccess))
	uint8 bIsFalling : 1;
	// ĳ���Ͱ� ���� �ޱ׷� �ɾ��ִ���
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SAnimInstance", meta = (AllowPrivateAccess))
	uint8 bIsCrouching : 1;

	// ���� ĳ������ ���ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SAnimInstance", meta = (AllowPrivateAccess))
	FVector Acceleration;

	// 'ELocomotionState' �Ӽ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ELocomotionState LocomotionState;
	// 'EMovementDirection' �Ӽ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EMovementDirection MovementDirection;

	// ��Ʈ�� �����̼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SAnimInstance", meta = (AllowPrivateAccess))
	FRotator ControlRotation;

	// ĳ���Ͱ� ���� �׾�����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SAnimInstance", meta = (AllowPrivateAccess))
	uint8 bIsDead : 1;
};
