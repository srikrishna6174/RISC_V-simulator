import { Toaster } from "@/components/ui/toaster";
import { Toaster as Sonner } from "@/components/ui/sonner";
import { TooltipProvider } from "@/components/ui/tooltip";
import { QueryClient, QueryClientProvider } from "@tanstack/react-query";
import { BrowserRouter, Routes, Route } from "react-router-dom";
import Index from "./pages/Index";
import TraverseCycle from "./pages/TraverseCycle";
import PipelineSimulation from "./pages/PipelineSimulation";
import Statistics from "./pages/Statistics";
import Memory from "./pages/Memory";
import NotFound from "./pages/NotFound";
import PipelineOverview from "./pages/PipelineOverview";

const queryClient = new QueryClient();

const App = () => (
  <QueryClientProvider client={queryClient}>
    <TooltipProvider>
      <Toaster />
      <Sonner />
      <BrowserRouter>
        <Routes>
          <Route path="/" element={<Index />} />
          <Route path="/traverse" element={<TraverseCycle />} />
          <Route path="/pipeline" element={<PipelineSimulation />} />
          <Route path="/pipeline-overview" element={<PipelineOverview />} />
          <Route path="/memory" element={<Memory />} />
          <Route path="/statistics" element={<Statistics />} />
          {/* ADD ALL CUSTOM ROUTES ABOVE THE CATCH-ALL "*" ROUTE */}
          <Route path="*" element={<NotFound />} />
        </Routes>
      </BrowserRouter>
    </TooltipProvider>
  </QueryClientProvider>
);

export default App;
