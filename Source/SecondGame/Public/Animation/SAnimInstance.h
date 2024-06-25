// SAnimInstance.h

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SAnimInstance.generated.h"

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
};
