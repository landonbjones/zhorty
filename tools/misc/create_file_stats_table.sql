DROP TABLE IF EXISTS file_stats;
CREATE TABLE file_stats (
  file_name varchar(256) NOT NULL,
  hands int DEFAULT 0,
  solve_time decimal(25,15) DEFAULT NULL,
  unwinable_hands int DEFAULT 0,
  unwinable_solve_time decimal(25,15) DEFAULT NULL,
  hard_hands int DEFAULT 0,
  challange_hands int DEFAULT 0,
  lowest_win_percent decimal(17,15) DEFAULT NULL
);