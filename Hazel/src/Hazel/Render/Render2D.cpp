#include "hzpch.h"
#include "Hazel/Render/Render2D.h"
#include "Hazel/Render/OrthographicCamera.h"
#include "Hazel/Render/VertexArray.h"
#include "Hazel/Render/Shader.h"
#include "Hazel/Render/RenderCommand.h"
#include "Hazel/Render/Texture.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Hazel {
	
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		// TODO: texid
		float TexIndex;
		float TilingFactor;
	};

	struct Render2DData
	{
		static const uint32_t MaxQuads    = 10000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices  = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray>  QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader>       TextureShader;
		//Ref<Texture2D>    WhiteTexture;

		uint32_t    QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr  = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		glm::vec4 QuadVertexPositions[4];

		Render2D::Statistics Stats;
	};

	static Render2DData s_Data;

	void Render2D::Init()
	{
		s_Data.QuadVertexArray  = VertexArray::Create();
		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" }
		});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
		
		uint32_t* quadIndices       = new uint32_t[s_Data.MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;
			offset += 4;
		}
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		auto whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		// Set first texture slot to 0
		s_Data.TextureSlots[0] = whiteTexture;

		//shader
		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;
		s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Render2D::ShutDown()
	{
		delete[] s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBufferBase = nullptr;
	}

	void Render2D::BeginScene(const OrthographicCamera & camera)
	{
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		startBatch();
	}

	void Render2D::EndScene()
	{
		Flush();
	}

	void Render2D::Flush()
	{
		if (s_Data.QuadIndexCount == 0)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			s_Data.TextureSlots[i]->Bind(i);
		}

		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		s_Data.Stats.DrawCalls++;
	}

	void Render2D::DrawQuad(const glm::vec2 & position, const glm::vec2 & size, const glm::vec4 & color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Render2D::DrawQuad(const glm::vec3 & position, const glm::vec2 & size, const glm::vec4 & color)
	{
		DrawQuad(position, size, s_Data.TextureSlots[0], 1.0f, color);
	}

	void Render2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tinitColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tinitColor);
	}

	void Render2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tinitColor)
	{
		DrawRotatedQuad(position, size, 0, texture, tilingFactor, tinitColor);
	}

	void Render2D::DrawQuad(const glm::mat4 & transform, const glm::vec4 & color)
	{
		DrawQuad(transform, s_Data.TextureSlots[0], 1.0f, color);
	}

	void Render2D::DrawQuad(const glm::mat4 & transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4 & tintColor)
	{
		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
			nextBatch();

		int32_t textureIndex = findTextureIndex(texture);
		if (textureIndex < 0) {
			if (s_Data.TextureSlotIndex >= s_Data.MaxTextureSlots)
				nextBatch();
			textureIndex = s_Data.TextureSlotIndex;
			s_Data.TextureSlots[textureIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		size_t quadVertexCount = 4;
		glm::vec2 textureCoords[] = { { 0.0f, 0.0f },{ 1.0f, 0.0f },{ 1.0f, 1.0f },{ 0.0f, 1.0f } };

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color    = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}
		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Render2D::DrawRotatedQuad(const glm::vec2 & position, const glm::vec2 & size, float rotation, const glm::vec4 & color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Render2D::DrawRotatedQuad(const glm::vec3 & position, const glm::vec2 & size, float rotation, const glm::vec4 & color)
	{
		DrawRotatedQuad(position, size, rotation, s_Data.TextureSlots[0], 1.0f, color);
	}

	void Render2D::DrawRotatedQuad(const glm::vec2 & position, const glm::vec2 & size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4 & tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Render2D::DrawRotatedQuad(const glm::vec3 & position, const glm::vec2 & size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4 & tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0, 0.0, 1.0f })//»∆z÷·
			* glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Render2D::ResetStats()
	{
		s_Data.Stats.DrawCalls = 0;
		s_Data.Stats.QuadCount = 0;
	}
	int32_t Render2D::findTextureIndex(const Ref<Texture2D>& texture)
	{
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *texture)
			{
				return i;
			}
		}
		return -1;
	}

	const Render2D::Statistics& Render2D::GetStats()
	{
		return s_Data.Stats;
	}

	void Render2D::startBatch()
	{
		s_Data.QuadIndexCount      = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotIndex = 1;
	}

	void Render2D::nextBatch()
	{
		Flush();
		startBatch();
	}

}