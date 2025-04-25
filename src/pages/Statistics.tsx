import React, { useEffect, useState } from "react";
import Navigation from "@/components/Navigation";
import { ChartContainer } from "@/components/ui/chart";
import { PieChart, Pie, Cell, Tooltip } from "recharts";

interface StatisticsData {
  total_cycles: number;
  total_instructions: number;
  cpi: number;
  data_transfer_instructions: number;
  alu_instructions: number;
  control_instructions: number;
  stall_count: number;
  data_hazards: number;
  control_hazards: number;
  branch_mispredictions: number;
  stalls_data_hazards: number;
  stalls_control_hazards: number;
}

const Statistics: React.FC = () => {
  const [stats, setStats] = useState<StatisticsData | null>(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    const fetchData = async () => {
      try {
        const response = await fetch("../web3.txt");
        if (!response.ok) {
          throw new Error("Failed to fetch statistics data");
        }
        const data = await response.json();
        setStats(data);
        setLoading(false);
      } catch (err) {
        console.error("Error fetching statistics:", err);
        setError("Failed to load statistics data");
        setLoading(false);
      }
    };

    fetchData();
  }, []);

  if (loading) {
    return (
      <div className="min-h-screen w-full bg-background text-foreground">
        <Navigation />
        <div className="flex items-center justify-center min-h-[50vh]">
          <div className="text-center animate-pulse">
            <p className="text-lg">Loading statistics...</p>
          </div>
        </div>
      </div>
    );
  }

  if (error || !stats) {
    return (
      <div className="min-h-screen w-full bg-background text-foreground">
        <Navigation />
        <div className="max-w-7xl mx-auto px-4 sm:px-6 py-8">
          <div className="bg-destructive/10 text-destructive p-4 rounded-lg">
            <p>{error || "Failed to load statistics"}</p>
          </div>
        </div>
      </div>
    );
  }

  const instructionTypeData = [
    { name: "Data Transfer", value: stats.data_transfer_instructions },
    { name: "ALU", value: stats.alu_instructions },
    { name: "Control", value: stats.control_instructions },
  ];

  const COLORS = ["#222222", "#444444", "#666666"];

  return (
    <div className="min-h-screen w-full bg-background text-foreground transition-colors duration-300">
      <Navigation />
      <main className="max-w-7xl mx-auto px-4 sm:px-6 py-8">
        <div className="grid grid-cols-1 md:grid-cols-2 gap-8 mb-8">
          <div className="p-6 rounded-lg border bg-card text-card-foreground shadow-sm">
            <h2 className="text-xl font-semibold mb-4">Instruction Types</h2>
            <ChartContainer config={{}} className="w-full aspect-square">
              <PieChart>
                <Pie
                  data={instructionTypeData}
                  dataKey="value"
                  nameKey="name"
                  cx="50%"
                  cy="50%"
                  outerRadius={80}
                  label
                >
                  {instructionTypeData.map((entry, index) => (
                    <Cell key={`cell-${index}`} fill={COLORS[index % COLORS.length]} />
                  ))}
                </Pie>
                <Tooltip />
              </PieChart>
            </ChartContainer>
          </div>

          <div className="p-6 rounded-lg border bg-card text-card-foreground shadow-sm">
            <h2 className="text-xl font-semibold mb-4">Hazards & Stalls</h2>
            <div className="grid grid-cols-1 sm:grid-cols-2 gap-4">
              <div className="p-4 rounded-md bg-primary/10">
                <h3 className="text-sm font-medium text-muted-foreground">Data Hazards</h3>
                <p className="text-2xl font-bold">{stats.data_hazards}</p>
              </div>
              <div className="p-4 rounded-md bg-primary/10">
                <h3 className="text-sm font-medium text-muted-foreground">Control Hazards</h3>
                <p className="text-2xl font-bold">{stats.control_hazards}</p>
              </div>
              <div className="p-4 rounded-md bg-primary/10">
                <h3 className="text-sm font-medium text-muted-foreground">Branch Mispredictions</h3>
                <p className="text-2xl font-bold">{stats.branch_mispredictions}</p>
              </div>
              <div className="p-4 rounded-md bg-primary/10">
                <h3 className="text-sm font-medium text-muted-foreground">Total Stalls</h3>
                <p className="text-2xl font-bold">{stats.stall_count}</p>
              </div>
            </div>
          </div>
        </div>

        <div className="p-6 rounded-lg border bg-card text-card-foreground shadow-sm mb-8">
          <h2 className="text-xl font-semibold mb-4">Performance Summary</h2>
          <div className="grid grid-cols-1 sm:grid-cols-2 md:grid-cols-3 gap-4">
            <div className="p-4 rounded-md bg-primary/10">
              <h3 className="text-sm font-medium text-muted-foreground">Total Cycles</h3>
              <p className="text-2xl font-bold">{stats.total_cycles}</p>
            </div>
            <div className="p-4 rounded-md bg-primary/10">
              <h3 className="text-sm font-medium text-muted-foreground">Instructions Executed</h3>
              <p className="text-2xl font-bold">{stats.total_instructions}</p>
            </div>
            <div className="p-4 rounded-md bg-primary/10">
              <h3 className="text-sm font-medium text-muted-foreground">CPI</h3>
              <p className="text-2xl font-bold">{stats.cpi}</p>
            </div>
          </div>
        </div>

        <div className="flex flex-col items-center justify-center mb-8">
          <h2 className="text-xl font-semibold mb-2 text-center">Detailed Statistics</h2>
          <div className="bg-card/80 px-6 py-4 rounded-lg shadow border w-full max-w-md mx-auto">
            <div className="space-y-1 text-sm">
              <div className="flex justify-between items-center gap-2 font-medium">
                <span>Total Cycles:</span> <span>{stats.total_cycles}</span>
              </div>
              <div className="flex justify-between items-center gap-2">
                <span>Total Instructions Executed:</span> <span>{stats.total_instructions}</span>
              </div>
              <div className="flex justify-between items-center gap-2">
                <span>Cycles Per Instruction (CPI):</span> <span>{stats.cpi}</span>
              </div>
              <div className="flex justify-between items-center gap-2">
                <span>Data-transfer Instructions:</span> <span>{stats.data_transfer_instructions}</span>
              </div>
              <div className="flex justify-between items-center gap-2">
                <span>ALU Instructions:</span> <span>{stats.alu_instructions}</span>
              </div>
              <div className="flex justify-between items-center gap-2">
                <span>Control Instructions:</span> <span>{stats.control_instructions}</span>
              </div>
              <div className="flex justify-between items-center gap-2">
                <span>Total Stalls/Bubbles:</span> <span>{stats.stall_count}</span>
              </div>
              <div className="flex justify-between items-center gap-2">
                <span>Data Hazards:</span> <span>{stats.data_hazards}</span>
              </div>
              <div className="flex justify-between items-center gap-2">
                <span>Control Hazards:</span> <span>{stats.control_hazards}</span>
              </div>
              <div className="flex justify-between items-center gap-2">
                <span>Branch Mispredictions:</span> <span>{stats.branch_mispredictions}</span>
              </div>
              <div className="flex justify-between items-center gap-2">
                <span>Stalls due to Data Hazards:</span> <span>{stats.stalls_data_hazards}</span>
              </div>
              <div className="flex justify-between items-center gap-2">
                <span>Stalls due to Control Hazards:</span> <span>{stats.stalls_control_hazards}</span>
              </div>
            </div>
          </div>
        </div>
      </main>
    </div>
  );
};

export default Statistics;
