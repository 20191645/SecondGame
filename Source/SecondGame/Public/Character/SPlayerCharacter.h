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

	// Input Value 속성(Forward, Right) Getter 함수
	float GetForwardInputValue() const { return ForwardInputValue; }
	float GetRightInputValue() const { return RightInputValue; }

	virtual void Tick(float DeltaSeconds) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// 'CurrentBulletCount' Getter, Setter 함수
	int32 GetCurrentBulletCount(int32 InWeaponClassNumber) const { return CurrentBulletCount[InWeaponClassNumber - 1]; }
	void SetCurrentBulletCount(int32 InWeaponClassNumber, int32 InCurrentBulletCount);
	// 'MaxBulletCount' Getter, Setter 함수
	int32 GetMaxBulletCount(int32 InWeaponClassNumber) const { return MaxBulletCount[InWeaponClassNumber - 1]; }

	// 'WeaponClassNumber' Getter, Setter 함수
	int32 GetWeaponClassNumber() const { return WeaponClassNumber; }
	void SetWeaponClassNumber(int32 InWeaponClassNumber);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// 캐릭터 죽으면 호출되는 함수
	virtual void OnCharacterDeath() override;

	// 'OnFireEffect' 델리게이트에 바인드할 함수 -- Notify('FireEffect')에 반응
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

	// 'IA_Manual' 입력에 대응하는 함수
	void InputManual();

	// 'WeaponInstacne' 생성 
	// -- Server: 서버에서 수행
	UFUNCTION(Server, Reliable)
	void SpawnWeaponInstance_Server();
	// 'WeaponInstance' 파괴
	UFUNCTION(Server, Reliable)
	void DestroyWeaponInstance_Server();

	// 'WeaponInstance'가 복제될 때 호출되는 함수
	UFUNCTION()
	void OnRep_WeaponInstance();

	// 'WeaponClassNumber' 수정
	UFUNCTION(Server, Reliable)
	void SetWeaponClassNumber_Server(int32 InWeaponClassNumber);


	// 리스폰 이펙트 활성화 함수 -- 서버 접근용
	UFUNCTION(Server, Unreliable)
	void RespawnEffect_Server();
	// 리스폰 이펙트 활성화 함수 
	// -- NetMulticast: 서버에서 호출, 모든 PC에서 실행
	UFUNCTION(NetMulticast, Unreliable)
	void RespawnEffect_NetMulticast();

	// 사격 이펙트 활성화 함수 -- 서버 접근용
	UFUNCTION(Server, Unreliable)
	void FireEffect_Server();
	// 사격 이펙트 활성화 함수
	UFUNCTION(NetMulticast, Unreliable)
	void FireEffect_NetMulticast();

	// 'ForwardInputValue, RightInputValue' 업데이트
	UFUNCTION(Server, Unreliable)
	void UpdateInputValue_Server(const float& InForwardInputValue, const float& InRightInputValue);
	// 'CurrentAimPitch, CurrentAimYaw' 업데이트
	UFUNCTION(Server, Unreliable)
	void UpdateAimValue_Server(const float& InAimPitchValue, const float& InAimYawValue);

	// 총알 장전 애니메이션 재생 함수 -- 서버 접근용
	UFUNCTION(Server, Unreliable)
	void PlayReloadMontage_Server();
	// 총알 장전 애니메이션 재생 함수 -- Other Client
	UFUNCTION(NetMulticast, Unreliable)
	void PlayReloadMontage_NetMulticast();

	// 사격 애니메이션 재생 함수 -- 서버 접근용
	UFUNCTION(Server, Unreliable)
	void PlayFireMontage_Server();
	// 사격 애니메이션 재생 함수 -- Other Client
	UFUNCTION(NetMulticast, Unreliable)
	void PlayFireMontage_NetMulticast();

	// 데미지 처리 함수
	UFUNCTION(Server, Reliable)
	void ApplyDamage_Server(FHitResult HitResult, float HitDamage);

	// 피격 애니메이션 재생 함수 -- Owner, Other Client
	UFUNCTION(NetMulticast, Reliable)
	void PlayHitReactMontage_NetMulticast();

	// 플레이어 캐릭터 파괴 함수 -- 서버 접근용
	UFUNCTION(Server, Reliable)
	void DestroyPlayerCharacter_Server();
	// 플레이어 캐릭터 파괴 함수 -- Other Client
	UFUNCTION(NetMulticast, Reliable)
	void DestroyPlayerCharacter_NetMulticast();

public:
	// 'CurrentBulletCount'가 변화하면 BroadCast하는 델리게이트
	FOnCurrentBulletCountChangeDelegate OnCurrentBulletCountChangedDelegate;

	// 'WeaponClassNumber'가 변화하면 BroadCast하는 델리게이트
	FOnWeaponClassNumberChangeDelegate OnWeaponClassNumberChangedDelegate;

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
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float ForwardInputValue;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float RightInputValue;

	// 이전 Input Value 속성 값
	float PreviousForwardInputValue = 0.f;
	float PreviousRightInputValue = 0.f;

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
	UPROPERTY(ReplicatedUsing = OnRep_WeaponInstance, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
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
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	int32 WeaponClassNumber;

	// 카메라 흔들림 클래스 정보
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<UCameraShakeBase> FireCameraShake;

	// 무기별 현재 총알 개수
	TArray<int32> CurrentBulletCount;
	// 무기별 최대 총알 개수
	TArray<int32> MaxBulletCount;

	// 캐릭터 부활 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UParticleSystemComponent> RespawnParticleSystemComponent;

	// 사격 적중 효과 위젯 클래스 정보
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> FireHitEffectUIClass;
	// 사격 적중 효과 위젯 객체
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UUserWidget> FireHitEffectUIInstance;
};
