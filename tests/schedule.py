import unittest
import datetime as dt
import sunbeam

spe3 = sunbeam.parse('spe3/SPE3CASE1.DATA')

class TestSchedule(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        cls.spe3 = sunbeam.parse('spe3/SPE3CASE1.DATA')
        cls.sch = cls.spe3.schedule

    # def testWells(self):
    #     self.assertEqual(2, len(self.sch.wells))

    #     with self.assertRaises(KeyError):
    #         self.sch['foo']

    # def testContains(self):
    #     self.assertTrue('PROD' in self.sch)
    #     self.assertTrue('INJ'  in self.sch)
    #     self.assertTrue('NOT'  not in self.sch)
    #     self.assertFalse('NOT' in self.sch)

    # def testStartEnd(self):
    #     self.assertEqual(dt.date(2015, 1, 1),   self.sch.start)
    #     self.assertEqual(dt.date(2029, 12, 27), self.sch.end)

    # def testTimesteps(self):
    #     timesteps = self.sch.timesteps
    #     self.assertEqual(176, len(timesteps))
    #     self.assertEqual(dt.date(2015, 12, 31), timesteps[7])

    # def testGroups(self):
    #     g1 = self.sch.group()['G1'].wells
    #     self.assertEqual(2, len(g1))

    #     def head(xs): return next(iter(xs))

    #     inje = head(filter(sunbeam.Well.injector(0), g1))
    #     prod = head(filter(sunbeam.Well.producer(0), g1))

    #     self.assertEqual(self.sch['INJ'],  inje)
    #     self.assertEqual(self.sch['PROD'], prod)

    #     with self.assertRaises(KeyError):
    #         self.sch.group()['foo']
