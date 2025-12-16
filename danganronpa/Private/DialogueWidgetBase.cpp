#include "DialogueWidgetBase.h"
#include"Components/Button.h"
#include"Components/TextBlock.h"
#include"Components/Image.h"
#include<GeneralController.h>
#include "HUD/DTReader.h"
#include "Animation/WidgetAnimation.h"
#include"HUD/KototamaWidget.h"
#include<GeneralController.h>
#include "Kismet/GameplayStatics.h"
void UDialogueWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Dialogues.Empty();
	InitializePortraits();
	TextIndex = 0;

	if (MainDialogue)
	{
		MainDialogue->GetAllRows("TEST", Dialogues);//Dialogues已获取DataTable所有行信息
		UE_LOG(LogTemp, Warning, TEXT("Dialogues array size: %d"), Dialogues.Num());
		if (Dialogues.IsValidIndex(0)) {
			UE_LOG(LogTemp, Warning, TEXT("happened"));
			FDialogData* _data = MainDialogue->FindRow< FDialogData>("1", "TEST", false);//false代表不报错
			// 更新文本框内容
			DialogueText->SetText(FText::FromString(Dialogues[TextIndex]->DialogueText));
		}

	}
	else
		UE_LOG(LogTemp, Warning, TEXT("What's happened"));
	// ... 原有对话初始化代码 ...

	// ============ 新增：状态初始化 ============
	// 1. 初始化状态实例
	DialogueStateInstances.Empty();

	// 2. 创建默认状态实例
	CreateStateInstance(UDialoguePlayingState::StaticClass());
	CreateStateInstance(UDialogueHiddenState::StaticClass());
	CreateStateInstance(UDialogueInvestigateState::StaticClass());
	CreateStateInstance(UDialogueChoiceState::StaticClass());

	// 3. 设置初始状态为播放状态
	EnterPlayingState();
}


void UDialogueWidgetBase::OnBubbleClicked()
{
	// 如果地图结束，执行特殊逻辑
	if (EndMap) {
		// 地图结束的特殊处理
		return;
	}

	// 委托给当前状态处理
	if (CurrentDialogueState)
	{
		CurrentDialogueState->HandleBubbleClick(this);
	}
	else
	{
		// 保底逻辑：如果没有状态，使用原始逻辑
		OriginalOnBubbleClicked();
	}
}

void UDialogueWidgetBase::OriginalOnBubbleClicked()
{
	if (TextIndex < Dialogues.Num()) {
		if (Dialogues[TextIndex]->out) {
			UE_LOG(LogTemp, Warning, TEXT("Sensed Out"));
			SetPortraitHidden();
			SetTextAndBubbleHidden();

			// 切换到隐藏状态
			EnterHiddenState(4.0f);
		}
		else if (TextIndex + 1 < Dialogues.Num() && Dialogues[TextIndex + 1]->intro) {
			SetPortraitHidden();
			TextIndex++;
			SetPortraitVisible();
			DialogueText->SetText(FText::FromString(Dialogues[TextIndex]->DialogueText));
		}
		else {
			DialogueText->SetText(FText::FromString(Dialogues[++TextIndex]->DialogueText));
		}
	}
}

void UDialogueWidgetBase::SetTextAndBubbleHidden()
{
	TextBubble->SetVisibility(ESlateVisibility::Hidden);//按钮不可见且不可交互
	DialogueText->SetVisibility(ESlateVisibility::Hidden);
}

void UDialogueWidgetBase::SetTextAndBubbleVisible()
{
	TextBubble->SetVisibility(ESlateVisibility::Visible);
	DialogueText->SetVisibility(ESlateVisibility::Visible);
}

void UDialogueWidgetBase::InitializePortraits()
{
	if (TextBubble != nullptr)
	{
		TextBubble->OnClicked.AddDynamic(this, &UDialogueWidgetBase::OnBubbleClicked);
	}
}

void UDialogueWidgetBase::TextMoveOn(float delay)
{
	if (Dialogues[TextIndex + 1]->intro) {
		++TextIndex;
		UE_LOG(LogTemp, Warning, TEXT("CharacterName: %s"), *Dialogues[TextIndex]->CharacterName);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle1, this, &UDialogueWidgetBase::SetPortraitVisible, delay, false);
		DialogueText->SetText(FText::FromString(Dialogues[TextIndex]->DialogueText));
	}
	else {
		DialogueText->SetText(FText::FromString(Dialogues[++TextIndex]->DialogueText));
	}
	GetWorld()->GetTimerManager().SetTimer(TimerHandle2, this, &UDialogueWidgetBase::SetTextAndBubbleVisible, delay, false);
}

void UDialogueWidgetBase::SetPortraitVisible()
{
	UE_LOG(LogTemp, Warning, TEXT("SettingPortraitVisible!"));
	CurrentPortrait = Cast<UImage>(GetWidgetFromName(FName(Dialogues[TextIndex]->CharacterName)));
	//获取当前在说话的立绘名
	if (CurrentPortrait)
		CurrentPortrait->SetVisibility(ESlateVisibility::Visible);
}

void UDialogueWidgetBase::SetPortraitHidden()
{
	CurrentPortrait = Cast<UImage>(GetWidgetFromName(FName(Dialogues[TextIndex]->CharacterName)));
	//获取当前在说话的立绘名
	if (CurrentPortrait)
		CurrentPortrait->SetVisibility(ESlateVisibility::Hidden);
}// 状态管理核心方法
void UDialogueWidgetBase::ChangeDialogueState(TSubclassOf<UBaseDialogueState> NewStateClass)
{
    if (!NewStateClass)
        return;
    
    FString PreviousStateName = CurrentDialogueState ? 
        CurrentDialogueState->GetStateName() : TEXT("None");
    
    // 1. 退出当前状态
    if (CurrentDialogueState)
    {
        CurrentDialogueState->ExitState(this);
    }
    
    // 2. 获取或创建新状态实例
    FString StateClassName = NewStateClass->GetName();
    UBaseDialogueState* NewState = nullptr;
    
    if (DialogueStateInstances.Contains(StateClassName))
    {
        NewState = DialogueStateInstances[StateClassName];
    }
    else
    {
        NewState = NewObject<UBaseDialogueState>(this, NewStateClass);
        if (NewState)
        {
            DialogueStateInstances.Add(StateClassName, NewState);
        }
    }
    
    // 3. 切换状态
    if (NewState)
    {
        CurrentDialogueState = NewState;
        CurrentDialogueState->EnterState(this);
        
        UE_LOG(LogTemp, Warning, TEXT("State changed: %s -> %s"), 
               *PreviousStateName, *CurrentDialogueState->GetStateName());
    }
}

// 状态快捷方法
void UDialogueWidgetBase::EnterPlayingState()
{
    ChangeDialogueState(UDialoguePlayingState::StaticClass());
}

void UDialogueWidgetBase::EnterInvestigateState(UDataTable* InvestigationTable)
{
    ChangeDialogueState(UDialogueInvestigateState::StaticClass());
    
    // 设置调查数据
    if (CurrentDialogueState)
    {
        UDialogueInvestigateState* InvestigateState = 
            Cast<UDialogueInvestigateState>(CurrentDialogueState);
        if (InvestigateState && InvestigationTable)
        {
            InvestigateState->SetInvestigationData(InvestigationTable);
        }
    }
}

void UDialogueWidgetBase::EnterChoiceState(const TArray<FString>& ChoiceTexts, 
                                           const TArray<FString>& ChoiceTargets)
{
    ChangeDialogueState(UDialogueChoiceState::StaticClass());
    
    // 设置选项
    if (CurrentDialogueState)
    {
        UDialogueChoiceState* ChoiceState = 
            Cast<UDialogueChoiceState>(CurrentDialogueState);
        if (ChoiceState)
        {
            ChoiceState->SetChoices(ChoiceTexts, ChoiceTargets);
        }
    }
}

void UDialogueWidgetBase::EnterHiddenState(float AutoReturnDelay)
{
    ChangeDialogueState(UDialogueHiddenState::StaticClass());
    
    // 设置自动返回延迟
    if (CurrentDialogueState)
    {
        UDialogueHiddenState* HiddenState = 
            Cast<UDialogueHiddenState>(CurrentDialogueState);
        if (HiddenState)
        {
            HiddenState->SetAutoReturnDelay(AutoReturnDelay);
        }
    }
}

// 状态管理核心方法
void UDialogueWidgetBase::ChangeDialogueState(TSubclassOf<UBaseDialogueState> NewStateClass)
{
    if (!NewStateClass)
        return;

    FString PreviousStateName = CurrentDialogueState ?
        CurrentDialogueState->GetStateName() : TEXT("None");

    // 1. 退出当前状态
    if (CurrentDialogueState)
    {
        CurrentDialogueState->ExitState(this);
    }

    // 2. 获取或创建新状态实例
    FString StateClassName = NewStateClass->GetName();
    UBaseDialogueState* NewState = nullptr;

    if (DialogueStateInstances.Contains(StateClassName))
    {
        NewState = DialogueStateInstances[StateClassName];
    }
    else
    {
        NewState = NewObject<UBaseDialogueState>(this, NewStateClass);
        if (NewState)
        {
            DialogueStateInstances.Add(StateClassName, NewState);
        }
    }

    // 3. 切换状态
    if (NewState)
    {
        CurrentDialogueState = NewState;
        CurrentDialogueState->EnterState(this);

        UE_LOG(LogTemp, Warning, TEXT("State changed: %s -> %s"),
            *PreviousStateName, *CurrentDialogueState->GetStateName());
    }
}

// 状态快捷方法
void UDialogueWidgetBase::EnterPlayingState()
{
    ChangeDialogueState(UDialoguePlayingState::StaticClass());
}

void UDialogueWidgetBase::EnterInvestigateState(UDataTable* InvestigationTable)
{
    ChangeDialogueState(UDialogueInvestigateState::StaticClass());

    // 设置调查数据
    if (CurrentDialogueState)
    {
        UDialogueInvestigateState* InvestigateState =
            Cast<UDialogueInvestigateState>(CurrentDialogueState);
        if (InvestigateState && InvestigationTable)
        {
            InvestigateState->SetInvestigationData(InvestigationTable);
        }
    }
}

void UDialogueWidgetBase::EnterChoiceState(const TArray<FString>& ChoiceTexts,
    const TArray<FString>& ChoiceTargets)
{
    ChangeDialogueState(UDialogueChoiceState::StaticClass());

    // 设置选项
    if (CurrentDialogueState)
    {
        UDialogueChoiceState* ChoiceState =
            Cast<UDialogueChoiceState>(CurrentDialogueState);
        if (ChoiceState)
        {
            ChoiceState->SetChoices(ChoiceTexts, ChoiceTargets);
        }
    }
}

void UDialogueWidgetBase::EnterHiddenState(float AutoReturnDelay)
{
    ChangeDialogueState(UDialogueHiddenState::StaticClass());

    // 设置自动返回延迟
    if (CurrentDialogueState)
    {
        UDialogueHiddenState* HiddenState =
            Cast<UDialogueHiddenState>(CurrentDialogueState);
        if (HiddenState)
        {
            HiddenState->SetAutoReturnDelay(AutoReturnDelay);
        }
    }
}




