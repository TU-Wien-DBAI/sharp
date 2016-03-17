#ifndef SHARP_NODETUPLESETMAPOVERLAY_H_
#define SHARP_NODETUPLESETMAPOVERLAY_H_

#include "NodeTupleSetMap.hpp"

namespace sharp
{
	class SHARP_LOCAL NodeTupleSetMapOverlay : public INodeTupleSetMap
	{
	public:
		NodeTupleSetMapOverlay(
				const INodeTupleSetMap &original,
				htd::vertex_t node,
				const ITupleSet &replacement);

		virtual ~NodeTupleSetMapOverlay() override;

		virtual ITupleSet &operator[](htd::vertex_t node) override;
		virtual ITupleSet &at(htd::vertex_t node) override;
		
		virtual const ITupleSet &operator[](htd::vertex_t node) const override;
		virtual const ITupleSet &at(htd::vertex_t node) const override;

		virtual bool contains(htd::vertex_t node) const override;
		
	private:
		const INodeTupleSetMap &map_;
		htd::vertex_t node_;
		const ITupleSet &replacement_;

	}; // class NodeTupleSetMapOverlay

} // namespace sharp

#endif // SHARP_NODETUPLESETMAPOVERLAY_H_
