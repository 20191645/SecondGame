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

	// Input Value 속성(Forward, Right) Getter 함수
	float GetForwardInputValue() const { return ForwardInputValue; }
	float GetRightInputValue() const { return RightInputValue; }

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	// 'IA_Move' 입력에 대응하는 함수
	void InputMove(const FInputActionValue& InValue);
	// 'IA_Look' 입력에 대응하는 함수
	void InputLook(const FInputActionValue& InValue);

	// 'IA_QuickSlot01' 입력에 대응하는 함수
	void InputQuickSlot01(const FInputActionValue& InValue);
	// 'IA_QuickSlot02' 입력에 대응하는 함수
	void InputQuickSlot02(const FInputActionValue& InValue);
	// 'IA_QuickSlot03' 입력에 대응하는 함수
	void InputQuickSlot03(const FInputActionValue& InValue);

protected:
	// SpringArmComponent: 3인칭 시점 카메라 구도 설정 돕는 컴포넌트 - 카메라 봉 길이, 컴포넌트 회전 설정
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	// CameraComponenet: 가상 세계 모습을 플레이어 화면에 전송하는 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UCameraComponent> CameraComponent;

	// 'IC_PlayerCharacter'와 바인드할 속성
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<USInputConfigData> PlayerCharacterInputConfigData;
	// 'IMC_PlayerCharacter'와 바인드할 데이터
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UInputMappingContext> PlayerCharacterInputMappingContext;
	
	// 애니메이션(AnimInstance)를 위해서 Input Value 속성(Forward, Right) 추가
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float ForwardInputValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float RightInputValue;

	// 무기 액터1 클래스 정보 -- 'Pistols_A'
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSubclassOf<ASWeaponActor> WeaponClass01;
	// 무기 액터2 클래스 정보 -- 'Assault_Rifle_A'
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSubclassOf<ASWeaponActor> WeaponClass02;
	// 무기 액터 개체
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<ASWeaponActor> WeaponInstance;
};
