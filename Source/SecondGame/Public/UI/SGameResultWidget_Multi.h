// SGameReulstWidget_Multi.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGameResultWidget_Multi.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class SECONDGAME_API USGameResultWidget_Multi : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	// 'ReturnButton' Ŭ�� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
	void OnReturnButtonClicked();
	// 'ExitButton' Ŭ�� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
	void OnExitButtonClicked();
	
public:
	// �÷��̾� ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> RankingText;

	// Ÿ��Ʋ ȭ�� ���ư��� ��ư
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UButton> ReturnButton;
	// ���� ���� ��ư
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UButton> ExitButton;
};
