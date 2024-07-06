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

	virtual void Tick(float DeltaSeconds) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// 캐릭터 죽으면 호출되는 함수
	virtual void OnCharacterDeath() override;

	// 'OnFireEffect' 델리게이트에 바인드할 함수
	UFUNCTION()
	void OnFireEffect();

	// 캐릭터 리스폰하는 함수
	void Respawn();

private:
	// 'IA_Move' 입력에 대응하는 함수
	void InputMove(const FInputActionValue& InValue);
	// 'IA_Look' 입력에 대응하는 함수
	void InputLook(const FInputActionValue& InValue);

	// 'IA_QuickSlot01' 입력에 대응하는 함수
	void InputQuickSlot01();
	// 'IA_QuickSlot02' 입력에 대응하는 함수
	void InputQuickSlot02();
	// 'IA_QuickSlot03' 입력에 대응하는 함수
	void InputQuickSlot03();

	// 'IA_Crouch' 입력에 대응하는 함수
	void InputCrouch();

	// 'IA_Attack' 입력에 대응하는 함수
	void InputAttack();

	// 단발 사격 함수
	void TryFire();

	// 'IA_Zoom' 입력 순간에 대응하는 줌인 함수
	void ZoomIn();
	// 'IA_Zoom' 입력 끝에 대응하는 줌인 함수
	void ZoomOut();

	// 'IA_Trigger' 입력에 대응하는 함수
	void ToggleTrigger();
	// 'IA_Attack' 입력 순간에 대응하는 함수
	void StartFire();
	// 'IA_Attack' 입력 끝에 대응하는 함수
	void StopFire();

	// 'IA_Reload' 입력에 대응하는 함수
	void InputReload();

protected:
	// SpringArmComponent: 3인칭 시점 카메라 구도 설정 돕는 컴포넌트
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

	// 무기 액터1 클래스 정보 -- 'Pistols_A' -- 줌X, 연발X
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSubclassOf<ASWeaponActor> WeaponClass01;
	// 무기 액터2 클래스 정보 -- 'Assault_Rifle_A' -- 줌X, 연발O
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSubclassOf<ASWeaponActor> WeaponClass02;
	// 무기 액터3 클래스 정보 -- 'Sniper_Rifle_A' -- 줌O, 연발X
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSubclassOf<ASWeaponActor> WeaponClass03;
	// 무기 액터 객체
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<ASWeaponActor> WeaponInstance;

	// 목표 FOV 값
	float TargetFOV = 70.f;
	// 현재 FOV 값
	float CurrentFOV = 70.f;

	// 연발 사격 시 분당 사격 횟수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	float FirePerMinute = 360;
	// 'IA_Trigger' 입력 시 값 반전 -- 단발/연발 토글링
	bool bIsTriggerToggle = false;
	// 연발 사격 타이머
	FTimerHandle BetweenShotsTimer;
	// 연발 사격 시간 간격
	float TimeBetweenFire;

	// 줌 위젯 클래스 정보
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> SniperZoomUIClass;
	// 줌 위젯 객체
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UUserWidget> SniperZoomUIInstance;

	// 현재 무기 클래스 번호
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	int32 WeaponClassNumber;

	// 카메라 흔들림 클래스 정보
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<UCameraShakeBase> FireCameraShake;

	// 무기별 현재 총알 개수
	TArray<int32> CurrentBulletCount;
	// 무기별 최대 총알 개수
	TArray<int32> MaxBulletCount;

	// 피격 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> HitReactAnimMontage;

	// 캐릭터 부활 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UParticleSystemComponent> RespawnParticleSystemComponent;
};
