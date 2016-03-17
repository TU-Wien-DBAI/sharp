#ifndef SHARP_NODETABLEMAPOVERLAY_H_
#define SHARP_NODETABLEMAPOVERLAY_H_

#include "NodeTableMap.hpp"

namespace sharp
{
	class SHARP_LOCAL NodeTableMapOverlay : public INodeTableMap
	{
	public:
		NodeTableMapOverlay(
				const INodeTableMap &original,
				htd::vertex_t node,
				const ITable &replacement);

		virtual ~NodeTableMapOverlay() override;

		virtual ITable &operator[](htd::vertex_t node) override;
		virtual ITable &at(htd::vertex_t node) override;

		virtual const ITable &operator[](htd::vertex_t node) const override;
		virtual const ITable &at(htd::vertex_t node) const override;

		virtual bool contains(htd::vertex_t node) const override;

	private:
		const INodeTableMap &map_;
		htd::vertex_t node_;
		const ITable &replacement_;

	}; // class NodeTableMapOverlay

} // namespace sharp

#endif // SHARP_NODETABLEMAPOVERLAY_H_
