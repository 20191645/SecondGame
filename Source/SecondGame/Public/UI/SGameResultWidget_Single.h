// SGameResultWidget_Single.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGameResultWidget_Single.generated.h"

class UButton;

UCLASS()
class SECONDGAME_API USGameResultWidget_Single : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USGameResultWidget_Single(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void NativeConstruct() override;

	// 'ReturnButton' Ŭ�� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
	void OnReturnButtonClicked();
	// 'ExitButton' Ŭ�� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
	void OnExitButtonClicked();

private:
	// Ÿ��Ʋ ȭ�� ���ư��� ��ư
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButton> ReturnButton;
	// ���� ���� ��ư
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButton> ExitButton;
};
