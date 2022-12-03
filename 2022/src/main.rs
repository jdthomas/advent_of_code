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

fn main() -> Result<()> {
    let args = Arguments::parse();

    let file = File::open(args.filename)?;
    let lines: Result<Vec<String>> = std::io::BufReader::new(file)
        .lines()
        .into_iter()
        .map(|x| x.map_err(anyhow::Error::from))
        .collect();
    let lines = lines?;

    match args.day {
        1 => {
            let now = Instant::now();
            let answer = aoc2022::day_1::solve_problem_1(lines.clone())?;
            println!(
                "Result = {}, resolved in {}usec\n",
                answer,
                now.elapsed().as_micros()
            );

            let now = Instant::now();
            let answer = aoc2022::day_1::solve_problem_2(lines)?;
            println!(
                "Result = {}, resolved in {}usec\n",
                answer,
                now.elapsed().as_micros()
            );
        }
        _ => {
            println!("Day not implemented yet");
        }
    }

    Ok(())
}
