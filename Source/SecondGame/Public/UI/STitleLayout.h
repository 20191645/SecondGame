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

	// 'SinglePlayButton' Ŭ�� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
	void OnSinglePlayButtonClicked();
	// 'MultiPlayButton' Ŭ�� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
	void OnMultiPlayButtonClicked();
	// 'ExitButton' Ŭ�� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
	void OnExitButtonClicked();

private:
	// 1�� �÷��� ���� ��ư
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USTitleLayout, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButton> SinglePlayButton;
	// ��Ƽ �÷��� ���� ��ư
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USTitleLayout, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButton> MultiPlayButton;
	// ���� ���� ��ư
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USTitleLayout, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButton> ExitButton;
};
