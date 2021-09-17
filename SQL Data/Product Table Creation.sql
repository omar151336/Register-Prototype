CREATE TABLE `products` (
  `InternalKey` int unsigned NOT NULL AUTO_INCREMENT,
  `SerialNumber` text,
  `Category` text,
  `Name` text NOT NULL,
  `Provider` text,
  `Cost` float DEFAULT '0',
  `Price` float DEFAULT NULL,
  `Profit` float GENERATED ALWAYS AS ((`Price` - `Cost`)) VIRTUAL,
  `QuantitySold` int DEFAULT '0',
  PRIMARY KEY (`InternalKey`),
  UNIQUE KEY `InternalKey_UNIQUE` (`InternalKey`)
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
