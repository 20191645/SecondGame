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
	// 단발 사격 함수
	void TryFire();
	// 연발 사격 함수
	void StartFire();

	// 'OnFireEffect' 델리게이트에 바인드할 함수
	UFUNCTION()
	void OnFireEffect();

	// 총알 장전 함수
	void Reload();

protected:
	// 무기 클래스 정보 -- 'Assault_Rifle_A'
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSubclassOf<ASWeaponActor> WeaponClass;
	// 무기 액터 객체
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<ASWeaponActor> WeaponInstance;

	// 분당 사격 횟수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	float FirePerMinute = 75;
	// 연발 사격 타이머
	FTimerHandle BetweenShotsTimer;
	// 연발 사격 시간 간격
	float TimeBetweenFire;

	// 최대 총알 개수
	int32 MaxBulletCount;

	// HP바 위젯 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<USWidgetComponent> WidgetComponent;
};
