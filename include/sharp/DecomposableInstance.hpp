#ifndef SHARP_SHARP_DECOMPOSABLEINSTANCE_H_
#define SHARP_SHARP_DECOMPOSABLEINSTANCE_H_

#include <sharp/global>

#include <sharp/IDecomposableInstance.hpp>

#include <memory>

namespace sharp
{
	template<typename T>
	class SHARP_API DecomposableInstance : public IDecomposableInstance
	{
	public:
		DecomposableInstance(const T &content) : content_(content) { }
		virtual ~DecomposableInstance() { };

		const T &content() const { return content_; }

		virtual htd::IHypergraph *toHypergraph() const override;

	private:
		const T &content_;

	}; // class DecomposableInstance

} // namespace sharp

#endif // SHARP_SHARP_DECOMPOSABLEINSTANCE_H_
