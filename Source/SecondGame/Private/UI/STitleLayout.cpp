// STitleLayout.cpp

#include "UI/STitleLayout.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

USTitleLayout::USTitleLayout(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void USTitleLayout::NativeConstruct()
{
	// 버튼 클릭 이벤트와 멤버 함수 바인드
	SinglePlayButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnSinglePlayButtonClicked);
	MultiPlayButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnMultiPlayButtonClicked);
	ExitButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnExitButtonClicked);
}

void USTitleLayout::OnSinglePlayButtonClicked()
{
	// 레벨로 이동
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("SingleMainLevel"));
}

void USTitleLayout::OnMultiPlayButtonClicked()
{
	// 레벨로 이동
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MultiMainLevel"));
}

void USTitleLayout::OnExitButtonClicked()
{
	// 게임 종료
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
