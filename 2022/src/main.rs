use anyhow::anyhow;
use anyhow::Result;
use clap::Parser;

use std::fs::File;
use std::io::BufRead;

use std::time::Instant;

#[derive(Debug, Parser)]
struct Arguments {
    day: u32,
    filename: String,
}

fn solve_problem_1(args: &Arguments, lines: &[String]) -> Result<i64> {
    match args.day {
        1 => aoc2022::day_01::solve_problem_1(lines),
        2 => aoc2022::day_02::solve_problem_1(lines),
        3 => aoc2022::day_03::solve_problem_1(lines),
        4 => aoc2022::day_04::solve_problem_1(lines),
        5 => aoc2022::day_05::solve_problem_1(lines),
        6 => aoc2022::day_06::solve_problem_1(lines),
        7 => aoc2022::day_07::solve_problem_1(lines),
        8 => aoc2022::day_08::solve_problem_1(lines),
        9 => aoc2022::day_09::solve_problem_1(lines),
        _ => Err(anyhow!("Day not implemented")),
    }
}

fn solve_problem_2(args: &Arguments, lines: &[String]) -> Result<i64> {
    match args.day {
        1 => aoc2022::day_01::solve_problem_2(lines),
        2 => aoc2022::day_02::solve_problem_2(lines),
        3 => aoc2022::day_03::solve_problem_2(lines),
        4 => aoc2022::day_04::solve_problem_2(lines),
        5 => aoc2022::day_05::solve_problem_2(lines),
        6 => aoc2022::day_06::solve_problem_2(lines),
        7 => aoc2022::day_07::solve_problem_2(lines),
        8 => aoc2022::day_08::solve_problem_2(lines),
        9 => aoc2022::day_09::solve_problem_2(lines),
        _ => Err(anyhow!("Day not implemented")),
    }
}

fn main() -> Result<()> {
    let args = Arguments::parse();

    let file = File::open(&args.filename)?;
    let lines: Result<Vec<String>> = std::io::BufReader::new(file)
        .lines()
        .into_iter()
        .map(|x| x.map_err(anyhow::Error::from))
        .collect();
    let lines = lines?;

    let now = Instant::now();
    let answer = solve_problem_1(&args, &lines)?;
    println!(
        "Result = {}, resolved in {}usec\n",
        answer,
        now.elapsed().as_micros()
    );

    let now = Instant::now();
    let answer = solve_problem_2(&args, &lines)?;
    println!(
        "Result = {}, resolved in {}usec\n",
        answer,
        now.elapsed().as_micros()
    );

    Ok(())
}
