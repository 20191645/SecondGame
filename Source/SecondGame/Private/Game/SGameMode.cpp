// SGameMode.cpp

#include "Game/SGameMode.h"
#include "Controller/SPlayerController.h"

ASGameMode::ASGameMode()
{
	// �÷��̾� ��Ʈ�ѷ� ����
	// ��Ƽ�÷��� ��� -- Class ������ �÷��̾� ���帶�� Instance�� ������
	PlayerControllerClass = ASPlayerController::StaticClass();
}
