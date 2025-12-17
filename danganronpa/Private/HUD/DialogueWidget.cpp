#include "HUD/DialogueWidget.h"
#include"Components/Button.h"
#include"Components/TextBlock.h"
#include"Components/Image.h"
#include<Player/NaegiController.h>
#include "HUD/DTReader.h"
#include "Animation/WidgetAnimation.h"
#include"HUD/KototamaWidget.h"
#include "Kismet/GameplayStatics.h"

void UDialogueWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InitializePortraits();
	FName _rowname = FName(TEXT("1"));
	FString ContextString = TEXT("TEST");


	if (MyDialogue)
	{
		MyDialogue->GetAllRows(ContextString, Dialogues);//Dialogues has obtained all information of DataTable
		FDialogData* _data = MyDialogue->FindRow< FDialogData>(_rowname, ContextString, false);//false means optional
		// Set initial text
		if (Dialogues.IsValidIndex(TextIndex))
        {
		    DialogueText->SetText(FText::FromString(Dialogues[TextIndex]->DialogueText));
        }
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("No connected DT"));
	}

    // Refactored with Decorator Pattern
    BuildPipeline(); // Assemble decorator chain
    Ctx = MakeUnique<FDlgContext>(); // Create context object
    Ctx->Widget = this; // Initialize context
    
}

// Refactored with Decorator Pattern
void UDialogueWidget::OnBubbleClicked()
{
    // Complex logic is now delegated to the decorator chain
    if (RootStep)
    {
        RootStep->Execute(*Ctx);
    }
}



void UDialogueWidget::OnCameraClicked()
{
	TextIndex = -1;
	CameraRead = true;
	if (InvestigatePhase) {
		FString ContextString = TEXT("TEST");
		Dialogues.Empty();
		CameraDialogue->GetAllRows(ContextString, Dialogues);
		SetTextAndBubbleVisible();
		DialogueText->SetText(FText::FromString(Dialogues[++TextIndex]->DialogueText));
	}

}

void UDialogueWidget::OnPaperClicked()
{
	TextIndex = -1; // Reset dialogue index
	PaperRead = true; // Mark Paper dialogue as read

	if (InvestigatePhase) {
		FString ContextString = TEXT("TEST");
		Dialogues.Empty();
		PaperDialogue->GetAllRows(ContextString, Dialogues);

		// Display the first sentence of Paper dialogue
		SetTextAndBubbleVisible();
		DialogueText->SetText(FText::FromString(Dialogues[++TextIndex]->DialogueText));
	}
}

void UDialogueWidget::PlayAnimate()
{
	if (PaperRead && CameraRead) {
		TextMoveOn(0.0f); // Trigger subsequent logic with delay
	}
}

void UDialogueWidget::SetTextAndBubbleHidden()
{
	TextBubble->SetVisibility(ESlateVisibility::Hidden);//Button invisibility can also be set
	DialogueText->SetVisibility(ESlateVisibility::Hidden);
}

void UDialogueWidget::SetTextAndBubbleVisible()
{
	TextBubble->SetVisibility(ESlateVisibility::Visible);
	DialogueText->SetVisibility(ESlateVisibility::Visible);
}

void UDialogueWidget::InitializePortraits()
{
	if (TextBubble != nullptr)
	{
		TextBubble->OnClicked.AddDynamic(this, &UDialogueWidget::OnBubbleClicked);
	}
	Broadcast->SetRenderOpacity(0.0f);
	Naegi_normal->SetVisibility(ESlateVisibility::Hidden);
	Naegi_talking->SetVisibility(ESlateVisibility::Hidden);
	Naegi_confused->SetVisibility(ESlateVisibility::Hidden);
	Camera->SetRenderOpacity(0.0f);
	Camera->OnClicked.AddDynamic(this, &UDialogueWidget::OnCameraClicked);
	Paper->SetRenderOpacity(0.0f);
	Paper->OnClicked.AddDynamic(this, &UDialogueWidget::OnPaperClicked);
}

void UDialogueWidget::TextMoveOn(float delay)
{
	// Ensure TextIndex locates to the dialogue with index 3
	int32 TargetIndex = 3;

	// Check if Dialogues is valid and elements with index 3 and 4 exist
	if (MyDialogue) {
		FString ContextString = TEXT("TEST");
		Dialogues.Empty();
		MyDialogue->GetAllRows(ContextString, Dialogues);

		if (Dialogues.IsValidIndex(TargetIndex) && Dialogues.IsValidIndex(TargetIndex + 1)) {
			// Display the 3rd dialogue
			TextIndex = TargetIndex;
			DialogueText->SetText(FText::FromString(Dialogues[TextIndex]->DialogueText));

			// Play animation and display dialogue
			PlayAnimation(BroadcastAppear);
			SetTextAndBubbleVisible();
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Target Index 3 or 4 is out of range in Dialogues!"));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("MyDialogue is NULL! Cannot load dialogues."));
	}
}

void UDialogueWidget::SetPortraitVisible()
{
	if (!Dialogues.IsValidIndex(TextIndex)) return;

	UE_LOG(LogTemp, Warning, TEXT("SettingPortraitVisible"));
	CurrentPortrait = Cast<UImage>(GetWidgetFromName(FName(Dialogues[TextIndex]->CharacterName)));
	//Get the name of the current speaker
	if (CurrentPortrait)
		CurrentPortrait->SetVisibility(ESlateVisibility::Visible);
}

void UDialogueWidget::SetPortraitHidden()
{
    if (!Dialogues.IsValidIndex(TextIndex)) return;

	CurrentPortrait = Cast<UImage>(GetWidgetFromName(FName(Dialogues[TextIndex]->CharacterName)));
	//Get the name of the current speaker
	if (CurrentPortrait)
		CurrentPortrait->SetVisibility(ESlateVisibility::Hidden);
}




//Refactored with Decorator Pattern

void UDialogueWidget::BuildPipeline()
{
    // Assemble decorator chain, order: from inside to outside
    // 1. Core is "display text and advance"
    TUniquePtr<IDlgStep> Core = MakeUnique<FCoreStep>();
    // 2. Wrap core with "portrait processor"
    TUniquePtr<IDlgStep> WithPortrait = MakeUnique<FPortraitDeco>(MoveTemp(Core));
    // 3. Wrap outermost layer with "input guard", called first
    TUniquePtr<IDlgStep> WithGuard = MakeUnique<FInputGuardDeco>(MoveTemp(WithPortrait));

    // Save the outermost decorator as the entry point
    RootStep = MoveTemp(WithGuard);
}

// Core step: display text and advance index
void FCoreStep::Execute(FDlgContext& Ctx)
{
    if (!Ctx.Widget) return;
    UDialogueWidget* W = Ctx.Widget;

    // Check if index is valid
    if (W->Dialogues.IsValidIndex(W->TextIndex))
    {
        // Only responsible for displaying text and advancing index
        W->DialogueText->SetText(FText::FromString(W->Dialogues[W->TextIndex]->DialogueText));
        W->TextIndex++; // Core: index +1
    }
    else
    {
        // If dialogue ends, you can add an end logic, left blank for now
        UE_LOG(LogTemp, Warning, TEXT("Dialogue ended or invalid index."));
        // Example: W->GetWorld()->GetFirstPlayerController<ADialoguePlayerController>()->EndDialogue();
    }
}

// Input guard: prohibit advancing during animation playback
void FInputGuardDeco::Execute(FDlgContext& Ctx)
{
    if (!Ctx.Widget) return;
    // If animation is playing, return directly without executing subsequent steps
    if (Ctx.Widget->bIsAnimating)
    {
        UE_LOG(LogTemp, Warning, TEXT("Input blocked by FInputGuardDeco."));
        return;
    }
    // Otherwise, execute the wrapped steps normally
    FDlgDecorator::Execute(Ctx);
}

// Portrait processing: display and hide the character portrait of the current line
void FPortraitDeco::Execute(FDlgContext& Ctx)
{
    if (!Ctx.Widget) return;
    UDialogueWidget* W = Ctx.Widget;

    // Display portrait before executing core steps
    // Note: Logic is simplified here, intro/out processing is not handled. Can be added as new decorators later.
    W->SetPortraitVisible();

    // Execute wrapped steps (e.g. FCoreStep)
    FDlgDecorator::Execute(Ctx);

    // After core steps are executed, portrait can be hidden if needed
    // W->SetPortraitHidden();
}

