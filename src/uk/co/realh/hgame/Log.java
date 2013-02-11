/*
 * Copyright (c) 2013, Tony Houghton <h@realh.co.uk>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * HGame - a simple game app framework targeted at Android.
 *
 * @author Tony Houghton &lt;h@realh.co.uk&gt;
 *
 * Distributed under the terms of the Simplified BSD Licence.
 * See the source code for details.
 */

package uk.co.realh.hgame;

/**
 * Provides a logging service modelled on Android's, but portable due to
 * abstraction.
 * 
 * @author Tony Houghton
 */
public class Log {
	
	/**
	 * Interface for an implementation.
	 */
	public static interface LogImplementation {
		
		public void wtf(String tag, String message);
		public void wtf(String tag, String message, Throwable ex);
		public void e(String tag, String message);
		public void e(String tag, String message, Throwable ex);
		public void w(String tag, String message);
		public void w(String tag, String message, Throwable ex);
		public void i(String tag, String message);
		public void i(String tag, String message, Throwable ex);
		public void d(String tag, String message);
		public void d(String tag, String message, Throwable ex);
		public void v(String tag, String message);
		public void v(String tag, String message, Throwable ex);
	}
	
	private static LogImplementation smLogger;
	
	/**
	 * Sets the implementation object which is of a derived class implementing
	 * the abstract methods. Must be called first.
	 * 
	 * @param logger	The logging object.
	 */
	public static void setLogger(LogImplementation logger)
	{
		smLogger = logger;
	}

	/**
	 * Log an assertion.
	 * 
	 * @param level
	 * @param tag
	 * @param message
	 */
	public static void wtf(String tag, String message)
	{
		smLogger.wtf(tag, message);
	}

	/**
	 * Log an assertion with exception.
	 * 
	 * @param level
	 * @param tag
	 * @param message
	 * @param ex
	 */
	public static void wtf(String tag, String message, Throwable ex)
	{
		smLogger.wtf(tag, message, ex);
	}
	
	/**
	 * Log an error.
	 * 
	 * @param level
	 * @param tag
	 * @param message
	 */
	public static void e(String tag, String message)
	{
		smLogger.e(tag, message);
	}

	/**
	 * Log an error with exception.
	 * 
	 * @param level
	 * @param tag
	 * @param message
	 * @param ex
	 */
	public static void e(String tag, String message, Throwable ex)
	{
		smLogger.e(tag, message, ex);
	}

	/**
	 * Log a warning.
	 * 
	 * @param level
	 * @param tag
	 * @param message
	 */
	public static void w(String tag, String message)
	{
		smLogger.w(tag, message);
	}

	/**
	 * Log a warning with exception.
	 * 
	 * @param level
	 * @param tag
	 * @param message
	 * @param ex
	 */
	public static void w(String tag, String message, Throwable ex)
	{
		smLogger.w(tag, message, ex);
	}
	
	/**
	 * Log info.
	 * 
	 * @param level
	 * @param tag
	 * @param message
	 */
	public static void i(String tag, String message)
	{
		smLogger.i(tag, message);
	}

	/**
	 * Log info with exception.
	 * 
	 * @param level
	 * @param tag
	 * @param message
	 * @param ex
	 */
	public static void i(String tag, String message, Throwable ex)
	{
		smLogger.i(tag, message, ex);
	}
	
	/**
	 * Log a debug message.
	 * 
	 * @param level
	 * @param tag
	 * @param message
	 */
	public static void d(String tag, String message)
	{
		smLogger.d(tag, message);
	}

	/**
	 * Log a debug message with exception.
	 * 
	 * @param level
	 * @param tag
	 * @param message
	 * @param ex
	 */
	public static void d(String tag, String message, Throwable ex)
	{
		smLogger.d(tag, message, ex);
	}
	
	/**
	 * Log a verbose debug message.
	 * 
	 * @param level
	 * @param tag
	 * @param message
	 */
	public static void v(String tag, String message)
	{
		smLogger.v(tag, message);
	}

	/**
	 * Log a verbose debug message with exception.
	 * 
	 * @param level
	 * @param tag
	 * @param message
	 * @param ex
	 */
	public static void v(String tag, String message, Throwable ex)
	{
		smLogger.v(tag, message, ex);
	}
	
	/**
	 * Log an error message then throw RunTimeException to force quit.
	 * 
	 * @param level
	 * @param tag
	 * @param message
	 */
	public static void f(String tag, String message)
	{
		smLogger.e(tag, message);
		throw new RuntimeException("Fatal error: " + message);
	}

	/**
	 * Log an error message then throw RunTimeException to force quit.
	 * 
	 * @param level
	 * @param tag
	 * @param message
	 */
	public static void f(String tag, String message, Throwable ex)
	{
		smLogger.e(tag, message, ex);
		throw new RuntimeException("Fatal exception: " + message + ": " +
				ex.toString());
	}

}
