use anyhow::anyhow;
use anyhow::Result;

#[derive(Debug)]
enum RPSResult {
    Win,
    Draw,
    Loss,
}
impl RPSResult {
    fn score(&self) -> i64 {
        match self {
            Self::Win => 6,
            Self::Draw => 3,
            Self::Loss => 0,
        }
    }

    fn try_from_str(s: &str) -> Result<Self> {
        Ok(match s.chars().next().unwrap() {
            'X' => Self::Loss,
            'Y' => Self::Draw,
            'Z' => Self::Win,

            _ => return Err(anyhow!("Bad move")),
        })
    }
}

#[derive(Debug, PartialEq, Eq, Clone)]
enum RPS {
    Rock,
    Paper,
    Sissors,
}
impl RPS {
    fn run_match(&self, other: &RPS) -> RPSResult {
        if self == other {
            return RPSResult::Draw;
        }
        match self {
            RPS::Rock => {
                if let RPS::Sissors = other {
                    RPSResult::Win
                } else {
                    RPSResult::Loss
                }
            }
            RPS::Paper => {
                if let RPS::Rock = other {
                    RPSResult::Win
                } else {
                    RPSResult::Loss
                }
            }
            RPS::Sissors => {
                if let RPS::Paper = other {
                    RPSResult::Win
                } else {
                    RPSResult::Loss
                }
            }
        }
    }

    fn for_result_play(&self, desired: &RPSResult) -> RPS {
        if let RPSResult::Draw = desired {
            return self.clone();
        }
        match self {
            RPS::Rock => {
                if let RPSResult::Loss = desired {
                    RPS::Sissors
                } else {
                    RPS::Paper
                }
            }
            RPS::Paper => {
                if let RPSResult::Loss = desired {
                    RPS::Rock
                } else {
                    RPS::Sissors
                }
            }
            RPS::Sissors => {
                if let RPSResult::Loss = desired {
                    RPS::Paper
                } else {
                    RPS::Rock
                }
            }
        }
    }

    fn try_from_str(s: &str) -> Result<Self> {
        Ok(match s.chars().next().unwrap() {
            'A' => Self::Rock,
            'B' => Self::Paper,
            'C' => Self::Sissors,

            'X' => Self::Rock,
            'Y' => Self::Paper,
            'Z' => Self::Sissors,
            _ => return Err(anyhow!("Bad move")),
        })
    }
    fn score(&self) -> i64 {
        match self {
            Self::Sissors => 3,
            Self::Paper => 2,
            Self::Rock => 1,
        }
    }
}

pub fn solve_problem_1(lines: &[String]) -> Result<i64> {
    let result: i64 = lines
        .iter()
        .map(|line| {
            let mut x = line.split(' ').map(RPS::try_from_str);
            let a = x.next().unwrap().unwrap();
            let b = x.next().unwrap().unwrap();
            b.run_match(&a).score() + b.score()
        })
        .into_iter()
        .sum();
    Ok(result)
}

pub fn solve_problem_2(lines: &[String]) -> Result<i64> {
    let result: i64 = lines
        .iter()
        .map(|line| {
            let mut x = line.split(' ');
            let a = RPS::try_from_str(x.next().unwrap()).unwrap();
            let b = RPSResult::try_from_str(x.next().unwrap()).unwrap();
            let play = a.for_result_play(&b);
            // println!("elf={:?} desired={:?}, so play={:?}", a, b, play);
            b.score() + play.score()
        })
        .into_iter()
        .sum();

    Ok(result)
}
