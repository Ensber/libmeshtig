inspect = require("inspect")

router = {
  nodes = {},
  links = {}
}

STATE_OPEN    = 0 -- cannot be processed
STATE_PENDING = 1 -- can be processed
STATE_VISITED = 2 -- already processed

for i = 1, 5 do
  table.insert(router.nodes, {
    links = {},
    visited = STATE_OPEN,
    score = 1/0,
    bestScoreFrom = 0
  })
end

function link(a,b, score)
  table.insert(router.nodes[a].links, b)
  table.insert(router.nodes[b].links, a)
  router.links[math.min(a,b) .. "-" .. math.max(a,b)] = {
    score = score
  }
end

link(1,2,1)
link(3,4,1)
link(1,4,1)
link(1,3,1)
link(2,4,1)
link(3,5,1)

function route(from, to)
  router.nodes[to].visited = STATE_PENDING
  router.nodes[to].score = 0
  while true do
    nPending = 0
    for i = 1, #router.nodes do
      if router.nodes[i].visited == STATE_PENDING then
        router.nodes[i].visited = STATE_VISITED
        for _, k in pairs(router.nodes[i].links) do
          linkScore = router.links[math.min(i,k) .. "-" .. math.max(i,k)].score + router.nodes[i].score
          if router.nodes[k].score > linkScore then
            router.nodes[k].score = linkScore
            router.nodes[k].bestScoreFrom = i
            router.nodes[k].visited = STATE_PENDING
            nPending = nPending + 1
          end
        end
      end
    end
    if nPending == 0 then
      break
    end
  end
  return router.nodes[from].bestScoreFrom
end

--[[
R1--R2
| \ |
R3--R4
|
R5
]]

from = 2
to   = 5
print(from, to)
repeat
  from = route(from, to)
  print(from, to)
until from == to

-- print("route", route(2,5))
-- print(inspect(router))
-- for i = 1, 