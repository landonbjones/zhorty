DROP TABLE IF EXISTS game_stats;

CREATE TABLE game_stats (
  hand char(52) NOT NULL,
  win_percent decimal(18,15) DEFAULT NULL,
  winable boolean DEFAULT NULL,
  straight_win boolean DEFAULT NULL,
  straight_last_win boolean DEFAULT NULL,
  solve_time decimal(19,15) DEFAULT NULL
);
