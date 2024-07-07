// STitleLayout.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STitleLayout.generated.h"

class UButton;

UCLASS()
class SECONDGAME_API USTitleLayout : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USTitleLayout(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void NativeConstruct() override;

	// 'SinglePlayButton' 클릭 시 호출되는 함수
	UFUNCTION()
	void OnSinglePlayButtonClicked();
	// 'MultiPlayButton' 클릭 시 호출되는 함수
	UFUNCTION()
	void OnMultiPlayButtonClicked();
	// 'ExitButton' 클릭 시 호출되는 함수
	UFUNCTION()
	void OnExitButtonClicked();

private:
	// 1인 플레이 시작 버튼
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USTitleLayout, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButton> SinglePlayButton;
	// 멀티 플레이 시작 버튼
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USTitleLayout, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButton> MultiPlayButton;
	// 게임 종료 버튼
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USTitleLayout, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButton> ExitButton;
};
