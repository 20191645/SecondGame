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
	// 로딩 레벨로 이동
	// -- 로딩 레벨에서는 'NextLevel'을 파싱해서 'SingleMainLevel' 값을 얻어낸다
	// OpenLevel() 함수의 Options 매개변수: FString(TEXT("NextLevel=SingleMainLevel"))
	// => 'NextLevel'이 Key, 'SingleMainLevel'이 Value 
	UGameplayStatics::OpenLevel(
		GetWorld(), FName(TEXT("LoadingLevel")), true, FString(TEXT("NextLevel=SingleLobbyLevel")));
}

void USTitleLayout::OnMultiPlayButtonClicked()
{
	// 로딩 레벨로 이동
	// -- 로딩 레벨에서는 'NextLevel'을 파싱해서 'MultiMainLevel' 값을 얻어낸다
	UGameplayStatics::OpenLevel(
		GetWorld(), FName(TEXT("LoadingLevel")), true, FString(TEXT("NextLevel=MultiLobbyLevel")));
}

void USTitleLayout::OnExitButtonClicked()
{
	// 게임 종료
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
