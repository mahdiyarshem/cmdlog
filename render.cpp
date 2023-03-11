#include <cmath>

#include <base/math.h>

#include "animstate.h"
#include "render.h"

#include <engine/graphics.h>
#include <engine/shared/config.h>

#include <game/generated/client_data.h>
#include <game/generated/client_data7.h>
#include <game/generated/protocol.h>

#include <game/mapitems.h>
#include <game/mapitems_ex.h>

static float gs_SpriteWScale;
static float gs_SpriteHScale;

void CRenderTools::Init(IGraphics *pGraphics, ITextRender *pTextRender)
{
	m_pGraphics = pGraphics;
	m_pTextRender = pTextRender;
	m_TeeQuadContainerIndex = Graphics()->CreateQuadContainer(false);
	Graphics()->SetColor(1.f, 1.f, 1.f, 1.f);

	Graphics()->QuadsSetSubset(0, 0, 1, 1);
	QuadContainerAddSprite(m_TeeQuadContainerIndex, 64.f);
	Graphics()->QuadsSetSubset(0, 0, 1, 1);
	QuadContainerAddSprite(m_TeeQuadContainerIndex, 64.f);

	Graphics()->QuadsSetSubset(0, 0, 1, 1);
	QuadContainerAddSprite(m_TeeQuadContainerIndex, 64.f * 0.4f);
	Graphics()->QuadsSetSubset(0, 0, 1, 1);
	QuadContainerAddSprite(m_TeeQuadContainerIndex, 64.f * 0.4f);
	Graphics()->QuadsSetSubset(0, 0, 1, 1);
	QuadContainerAddSprite(m_TeeQuadContainerIndex, 64.f * 0.4f);
	Graphics()->QuadsSetSubset(0, 0, 1, 1);
	QuadContainerAddSprite(m_TeeQuadContainerIndex, 64.f * 0.4f);
	Graphics()->QuadsSetSubset(0, 0, 1, 1);
	QuadContainerAddSprite(m_TeeQuadContainerIndex, 64.f * 0.4f);

	
	Graphics()->QuadsSetSubset(0, 0, 1, 1);
	QuadContainerAddSprite(m_TeeQuadContainerIndex, -32.f, -16.f, 64.f, 32.f);
	Graphics()->QuadsSetSubset(0, 0, 1, 1);
	QuadContainerAddSprite(m_TeeQuadContainerIndex, -32.f, -16.f, 64.f, 32.f);

	
	Graphics()->QuadsSetSubsetFree(1, 0, 0, 0, 0, 1, 1, 1);
	QuadContainerAddSprite(m_TeeQuadContainerIndex, -32.f, -16.f, 64.f, 32.f);
	Graphics()->QuadsSetSubsetFree(1, 0, 0, 0, 0, 1, 1, 1);
	QuadContainerAddSprite(m_TeeQuadContainerIndex, -32.f, -16.f, 64.f, 32.f);

	Graphics()->QuadContainerUpload(m_TeeQuadContainerIndex);
}

void CRenderTools::SelectSprite(CDataSprite *pSpr, int Flags, int sx, int sy)
{
	int x = pSpr->m_X + sx;
	int y = pSpr->m_Y + sy;
	int w = pSpr->m_W;
	int h = pSpr->m_H;
	int cx = pSpr->m_pSet->m_Gridx;
	int cy = pSpr->m_pSet->m_Gridy;

	GetSpriteScaleImpl(w, h, gs_SpriteWScale, gs_SpriteHScale);

	float x1 = x / (float)cx + 0.5f / (float)(cx * 32);
	float x2 = (x + w) / (float)cx - 0.5f / (float)(cx * 32);
	float y1 = y / (float)cy + 0.5f / (float)(cy * 32);
	float y2 = (y + h) / (float)cy - 0.5f / (float)(cy * 32);

	if(Flags & SPRITE_FLAG_FLIP_Y)
		std::swap(y1, y2);

	if(Flags & SPRITE_FLAG_FLIP_X)
		std::swap(x1, x2);

	Graphics()->QuadsSetSubset(x1, y1, x2, y2);
}

void CRenderTools::SelectSprite(int Id, int Flags, int sx, int sy)
{
	if(Id < 0 || Id >= g_pData->m_NumSprites)
		return;
	SelectSprite(&g_pData->m_aSprites[Id], Flags, sx, sy);
}

void CRenderTools::GetSpriteScale(client_data7::CDataSprite *pSprite, float &ScaleX, float &ScaleY)
{
	int w = pSprite->m_W;
	int h = pSprite->m_H;
	GetSpriteScaleImpl(w, h, ScaleX, ScaleY);
}

void CRenderTools::GetSpriteScale(struct CDataSprite *pSprite, float &ScaleX, float &ScaleY)
{
	int w = pSprite->m_W;
	int h = pSprite->m_H;
	GetSpriteScaleImpl(w, h, ScaleX, ScaleY);
}

void CRenderTools::GetSpriteScale(int Id, float &ScaleX, float &ScaleY)
{
	GetSpriteScale(&g_pData->m_aSprites[Id], ScaleX, ScaleY);
}

void CRenderTools::GetSpriteScaleImpl(int Width, int Height, float &ScaleX, float &ScaleY)
{
	float f = sqrtf(Height * Height + Width * Width);
	ScaleX = Width / f;
	ScaleY = Height / f;
}

void CRenderTools::DrawSprite(float x, float y, float Size)
{
	IGraphics::CQuadItem QuadItem(x, y, Size * gs_SpriteWScale, Size * gs_SpriteHScale);
	Graphics()->QuadsDraw(&QuadItem, 1);
}

void CRenderTools::DrawSprite(float x, float y, float ScaledWidth, float ScaledHeight)
{
	IGraphics::CQuadItem QuadItem(x, y, ScaledWidth, ScaledHeight);
	Graphics()->QuadsDraw(&QuadItem, 1);
}

void CRenderTools::RenderCursor(vec2 Center, float Size)
{
	Graphics()->WrapClamp();
	Graphics()->TextureSet(g_pData->m_aImages[IMAGE_CURSOR].m_Id);
	Graphics()->QuadsBegin();
	Graphics()->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	IGraphics::CQuadItem QuadItem(Center.x, Center.y, Size, Size);
	Graphics()->QuadsDrawTL(&QuadItem, 1);
	Graphics()->QuadsEnd();
	Graphics()->WrapNormal();
}

void CRenderTools::RenderIcon(int ImageId, int SpriteId, const CUIRect *pRect, const ColorRGBA *pColor)
{
	Graphics()->TextureSet(g_pData->m_aImages[ImageId].m_Id);
	Graphics()->QuadsBegin();
	SelectSprite(SpriteId);
	if(pColor)
		Graphics()->SetColor(pColor->r * pColor->a, pColor->g * pColor->a, pColor->b * pColor->a, pColor->a);
	IGraphics::CQuadItem QuadItem(pRect->x, pRect->y, pRect->w, pRect->h);
	Graphics()->QuadsDrawTL(&QuadItem, 1);
	Graphics()->QuadsEnd();
}

int CRenderTools::QuadContainerAddSprite(int QuadContainerIndex, float x, float y, float Size)
{
	IGraphics::CQuadItem QuadItem(x, y, Size, Size);
	return Graphics()->QuadContainerAddQuads(QuadContainerIndex, &QuadItem, 1);
}

int CRenderTools::QuadContainerAddSprite(int QuadContainerIndex, float Size)
{
	IGraphics::CQuadItem QuadItem(-(Size) / 2.f, -(Size) / 2.f, (Size), (Size));
	return Graphics()->QuadContainerAddQuads(QuadContainerIndex, &QuadItem, 1);
}

int CRenderTools::QuadContainerAddSprite(int QuadContainerIndex, float Width, float Height)
{
	IGraphics::CQuadItem QuadItem(-(Width) / 2.f, -(Height) / 2.f, (Width), (Height));
	return Graphics()->QuadContainerAddQuads(QuadContainerIndex, &QuadItem, 1);
}

int CRenderTools::QuadContainerAddSprite(int QuadContainerIndex, float X, float Y, float Width, float Height)
{
	IGraphics::CQuadItem QuadItem(X, Y, Width, Height);
	return Graphics()->QuadContainerAddQuads(QuadContainerIndex, &QuadItem, 1);
}

void CRenderTools::GetRenderTeeAnimScaleAndBaseSize(CAnimState *pAnim, CTeeRenderInfo *pInfo, float &AnimScale, float &BaseSize)
{
	AnimScale = pInfo->m_Size * 1.0f / 64.0f;
	BaseSize = pInfo->m_Size;
}

void CRenderTools::GetRenderTeeBodyScale(float BaseSize, float &BodyScale)
{
	BodyScale = g_Config.m_ClFatSkins ? BaseSize * 1.3f : BaseSize;
	BodyScale /= 64.0f;
}

void CRenderTools::GetRenderTeeFeetScale(float BaseSize, float &FeetScaleWidth, float &FeetScaleHeight)
{
	FeetScaleWidth = BaseSize / 64.0f;
	FeetScaleHeight = (BaseSize / 2) / 32.0f;
}

void CRenderTools::GetRenderTeeBodySize(CAnimState *pAnim, CTeeRenderInfo *pInfo, vec2 &BodyOffset, float &Width, float &Height)
{
	float AnimScale, BaseSize;
	GetRenderTeeAnimScaleAndBaseSize(pAnim, pInfo, AnimScale, BaseSize);

	float BodyScale;
	GetRenderTeeBodyScale(BaseSize, BodyScale);

	Width = pInfo->m_SkinMetrics.m_Body.WidthNormalized() * 64.0f * BodyScale;
	Height = pInfo->m_SkinMetrics.m_Body.HeightNormalized() * 64.0f * BodyScale;
	BodyOffset.x = pInfo->m_SkinMetrics.m_Body.OffsetXNormalized() * 64.0f * BodyScale;
	BodyOffset.y = pInfo->m_SkinMetrics.m_Body.OffsetYNormalized() * 64.0f * BodyScale;
}

void CRenderTools::GetRenderTeeFeetSize(CAnimState *pAnim, CTeeRenderInfo *pInfo, vec2 &FeetOffset, float &Width, float &Height)
{
	float AnimScale, BaseSize;
	GetRenderTeeAnimScaleAndBaseSize(pAnim, pInfo, AnimScale, BaseSize);

	float FeetScaleWidth, FeetScaleHeight;
	GetRenderTeeFeetScale(BaseSize, FeetScaleWidth, FeetScaleHeight);

	Width = pInfo->m_SkinMetrics.m_Feet.WidthNormalized() * 64.0f * FeetScaleWidth;
	Height = pInfo->m_SkinMetrics.m_Feet.HeightNormalized() * 32.0f * FeetScaleHeight;
	FeetOffset.x = pInfo->m_SkinMetrics.m_Feet.OffsetXNormalized() * 64.0f * FeetScaleWidth;
	FeetOffset.y = pInfo->m_SkinMetrics.m_Feet.OffsetYNormalized() * 32.0f * FeetScaleHeight;
}
