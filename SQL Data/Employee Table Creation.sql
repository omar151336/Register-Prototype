CREATE TABLE `employees` (
  `Key` int unsigned NOT NULL AUTO_INCREMENT,
  `Name` text NOT NULL,
  `Number` int NOT NULL DEFAULT '-1',
  `Code` int NOT NULL,
  `Privilege` int NOT NULL DEFAULT '-1',
  `TransactionsWTD` int DEFAULT '0',
  `TransactionsMTD` int DEFAULT '0',
  `RegisterOpensWTD` int DEFAULT '0',
  `RegisterOpensMTD` int DEFAULT '0',
  `MoneyMade` float DEFAULT '0',
  PRIMARY KEY (`Key`),
  UNIQUE KEY `Key_UNIQUE` (`Key`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
