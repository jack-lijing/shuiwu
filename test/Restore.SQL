-- MySQL dump 10.14  Distrib 5.5.36-MariaDB, for Linux (x86_64)
--
-- Host: jack    Database: test
-- ------------------------------------------------------
-- Server version	5.5.36-MariaDB-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `zlscnjcsb`
--

DROP TABLE IF EXISTS `zlscnjcsb`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `zlscnjcsb` (
  `cnjsl` decimal(5,4) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `zlscnjcsb`
--

LOCK TABLES `zlscnjcsb` WRITE;
/*!40000 ALTER TABLE `zlscnjcsb` DISABLE KEYS */;
INSERT INTO `zlscnjcsb` VALUES (0.1000);
/*!40000 ALTER TABLE `zlscnjcsb` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `zlsdab`
--

DROP TABLE IF EXISTS `zlsdab`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `zlsdab` (
  `Hh` char(11) DEFAULT NULL,
  `Xm` char(50) DEFAULT NULL,
  `Dz` char(50) DEFAULT NULL,
  `Nzh` char(20) DEFAULT NULL,
  `Sfzh` char(18) DEFAULT NULL,
  `Lxdh` char(50) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `zlsdab`
--

LOCK TABLES `zlsdab` WRITE;
/*!40000 ALTER TABLE `zlsdab` DISABLE KEYS */;
INSERT INTO `zlsdab` VALUES ('01234567890','郦晶','湖州市师范学院现代教育技术中心','33000333894567555555','330623198100010001','18268292601'),('00000000001','张三','湖州市师范学院技术部','22222222222222222222','330623198100010001','18268292601'),('00000000002','李四','湖州市师范学院技术部','33000333894567555555','330623198100010001','18268292601'),('11111111111','王五','统计局','0','','13333333333'),('22222222222','麻子','统计局','22222222222222222222','','13333333333');
/*!40000 ALTER TABLE `zlsdab` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `zlsjfczb`
--

DROP TABLE IF EXISTS `zlsjfczb`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `zlsjfczb` (
  `Hh` char(11) DEFAULT NULL,
  `Qkrq` char(6) DEFAULT NULL,
  `Je` decimal(12,2) DEFAULT NULL,
  `Cnje` decimal(12,2) DEFAULT NULL,
  `Jfczrq` char(8) DEFAULT NULL,
  `lsh` char(16) DEFAULT NULL,
  `cnjqsrq` char(8) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `zlsjfczb`
--

LOCK TABLES `zlsjfczb` WRITE;
/*!40000 ALTER TABLE `zlsjfczb` DISABLE KEYS */;
/*!40000 ALTER TABLE `zlsjfczb` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `zlsqkb`
--

DROP TABLE IF EXISTS `zlsqkb`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `zlsqkb` (
  `Hh` char(11) DEFAULT NULL,
  `Qkrq` char(6) DEFAULT NULL,
  `Je` decimal(12,2) DEFAULT NULL,
  `Cnje` decimal(12,2) DEFAULT NULL,
  `pkbz` int(11) DEFAULT NULL,
  `cnjqsrq` char(8) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `zlsqkb`
--

LOCK TABLES `zlsqkb` WRITE;
/*!40000 ALTER TABLE `zlsqkb` DISABLE KEYS */;
INSERT INTO `zlsqkb` VALUES ('11111111111','201306',87.30,2.30,1,'20130701'),('11111111111','201307',57.30,2.30,0,'20130801'),('11111111111','201308',44.30,2.30,0,'20130901'),('22222222222','201401',66.30,2.30,0,'20140201'),('22222222222','201402',56.30,2.30,0,'20140401');
/*!40000 ALTER TABLE `zlsqkb` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2014-04-01 15:26:59
