//
// Created by boris on 12/3/2021.
//

#ifndef VENGINE3D_VE_EPA_UNIQUE_EDGES_H
#define VENGINE3D_VE_EPA_UNIQUE_EDGES_H

namespace VE{
    namespace EPA{
        class UnigueEdge {
        public:
            void add(size_t a, size_t b) {
                auto reverse = std::find(
                        edges_.begin(),
                        edges_.end(),
                        std::make_pair(b, a)
                );

                if (reverse != edges_.end()) {
                    edges_.erase(reverse);
                } else {
                    edges_.emplace_back(a, b);
                }
            }

            const std::vector<std::pair<size_t, size_t>> edges() const {
                return edges_;
            }

            void clear(){
                edges_.clear();
            }
        private:
            std::vector<std::pair<size_t, size_t>> edges_;
        };
    }
}

#endif //VENGINE3D_VE_EPA_UNIQUE_EDGES_H
