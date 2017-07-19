#include "TextMaster.h"

#include "FontRenderer.h"
#include "Loader.h"
#include "TextMeshData.h"

Loader* TextMaster::ourLoader = nullptr;
std::unique_ptr<FontRenderer> TextMaster::ourRenderer = nullptr;
std::map<FontType*, std::vector<GUIText*>> TextMaster::ourTexts;

void TextMaster::Init(Loader* aLoader)
{
	ourLoader = aLoader;
	ourRenderer = std::make_unique<FontRenderer>();
}

void TextMaster::Render()
{
	ourRenderer->Render(ourTexts);
}

void TextMaster::LoadText(GUIText* someText)
{
	FontType& font = someText->GetFont();
	TextMeshData data = font.LoadText(someText);
	int vao = ourLoader->LoadToVAO(data.GetVertexPositions(), data.GetTextureCoords());
	someText->SetMeshInfo(vao, data.GetVertexCount());
	auto batch = ourTexts.find(&font);
	if (batch == ourTexts.end())
		ourTexts[&font] = std::vector<GUIText*>();

	ourTexts[&font].push_back(someText);
}

void TextMaster::RemoveText(GUIText* aGUIText)
{
	auto batch = ourTexts.find(&aGUIText->GetFont());
	if (batch != ourTexts.end())
	{		
		batch->second.erase(std::find(batch->second.begin(), batch->second.end(), aGUIText));
	}

	if (batch->second.size() == 0)
	{
		ourTexts.erase(batch);
		// TODO:CW DELETA VAO and VBOs?
	}
}

void TextMaster::CleanUp()
{
	//ourRenderer->CleanUp();
	ourRenderer = nullptr;
}