#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <queue>
#include <vector>

class Dijkstra {
public:
	void initialize(int32_t node_count, int32_t cost_unreachable = 100000) {
		m_node_previous.resize(node_count);
		m_node_cost.resize(node_count);

		std::ranges::fill(m_node_previous, 1);
		std::ranges::fill(m_node_cost, cost_unreachable);
	}

	void execute(int32_t source, std::function<std::vector<int32_t> const &(int32_t)> neighbours) {
		using prio_t = std::pair<int32_t, int32_t>;
		std::priority_queue<prio_t, std::vector<prio_t>, std::less<prio_t>> todo;
		todo.push(std::make_pair(0, source));

		while (!todo.empty()) {
			auto [curr_cost, curr_index] = todo.top();
			todo.pop();

			// check if this is a cheaper way to reach our neighbouring cells
			for (auto const &next_index : neighbours(curr_index)) {
				auto next_cost  = curr_cost + 1;

				if (next_cost < m_node_cost[next_index]) {
					todo.push(std::make_pair(next_cost, next_index));
					m_node_cost[next_index] = next_cost;
					m_node_previous[next_index] = curr_index;
				}
			}
		}
	}

	std::vector<int32_t> const &node_costs() {return m_node_cost;}

private:
	std::vector<int32_t>	m_node_previous;
	std::vector<int32_t>	m_node_cost;

};

